// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Weapon/WeaponSword.h"
#include "Actors/Character/PlayerCharacter.h"
#include "Actors/Monster/CharacterMonster.h"
#include "Actors/Monster/PawnMonster.h"

#include "Components/SkeletalMeshComponent.h"
#include "Components/StatusComponent/PlayerStatusComponent/PlayerStatusComponent.h"

#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "Kismet/KismetSystemLibrary.h"

#include "GameFramework/PawnMovementComponent.h"

#include "Engine/DamageEvents.h"

AWeaponSword::AWeaponSword()
{
    {


        PrimaryActorTick.bCanEverTick = true;

        DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
        RootComponent = DefaultSceneRoot;
        
        

        ConstructorHelpers::FObjectFinder<UStaticMesh> Asset(TEXT("/Script/Engine.StaticMesh'/Game/Resources/Weapon/SwordStaticMesh/Sword001.Sword001'"));

        StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
        StaticMeshComponent->SetupAttachment(RootComponent);
        StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        StaticMeshComponent->SetStaticMesh(Asset.Object);
    }
    for (int32 i = 1; i <= 4; ++i)
    {
        FString IndexStr = FString::FromInt(i);

        FString Path = FString::Printf(
            TEXT("/Script/Engine.AnimMontage'/Game/Resources/Player/Armor/Animation/NormalAttack/Sword_Attack_Montage_S%d.Sword_Attack_Montage_S%d'"),
            i, i
        );

        ConstructorHelpers::FObjectFinder<UAnimMontage> Montage(*Path);
        if (Montage.Succeeded())
        {
            Arr_Sword_Attack_MTG.Add(Montage.Object);
            MaxComboIndex += 1;
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Montage %d 로드 실패!"), i);
        }
    }
    
    {

        ConstructorHelpers::FObjectFinder<UAnimMontage> Asset(TEXT("/Script/Engine.AnimMontage'/Game/Resources/Player/Sword/Animation/Equip_Sword_On_Natural_Montage.Equip_Sword_On_Natural_Montage'"));
        if (Asset.Object)
        {
            EquipMontage = Asset.Object;
        }
    }
    {

        ConstructorHelpers::FObjectFinder<UAnimMontage> Asset(TEXT("/Script/Engine.AnimMontage'/Game/Resources/Player/Sword/Animation/Equip_Sword_Off_Montage.Equip_Sword_Off_Montage'"));
        if (Asset.Object)
        {
            UnEquipMontage = Asset.Object;
        }
    }
    {
        NiagaraEffectComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));
        ConstructorHelpers::FObjectFinder<UNiagaraSystem> Asset(TEXT("/Script/Niagara.NiagaraSystem'/Game/Vefects/FxER_StylizedSlash/Niagara/Stylize/NS_sm01_Stylized_Slash_04_B.NS_sm01_Stylized_Slash_04_B'"));
        if (Asset.Object)
        {
            NiagaraEffectComponent->SetAsset(Asset.Object);
        }
        
    }

}

void AWeaponSword::LeftClickAction()
{
    if (!bCanAttack)return;

    AActor* OwnerActor = GetOwner();
    // 현재 액터 위치

    {
        APlayerCharacter* Player_C = Cast<APlayerCharacter>(OwnerActor);
        UAnimInstance* AnimInstance= Player_C->GetMesh()->GetAnimInstance();
        UAnimMontage* PlayingMontage = AnimInstance->GetCurrentActiveMontage();
        if (PlayingMontage)
        {
            if(PlayingMontage != Arr_Sword_Attack_MTG[PrevComboIndex])
                return;
        }
        
        AnimInstance->Montage_Play(Arr_Sword_Attack_MTG[CurrentComboIndex]);
        PrevComboIndex = CurrentComboIndex;
        CurrentComboIndex += 1;
        CurrentComboIndex = (MaxComboIndex <= CurrentComboIndex) ? 0 : CurrentComboIndex;
        Player_C->GetCharacterMovement()->SetMovementMode(MOVE_None);

    }

    NiagaraEffectComponent->AttachToComponent(StaticMeshComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
    bCanAttack = false;
    
}


void AWeaponSword::SetCanAttack()
{
    bCanAttack = true;

}

void AWeaponSword::SetCanMove()
{
    AActor* ActorPlayer = GetOwner();
    ACharacter* Character = Cast<ACharacter>(ActorPlayer);

    Character->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
}

void AWeaponSword::EmptyDamagedActors()
{
    DamagedActors.Empty(); // 공격 시작 시 클리어
}

void AWeaponSword::Attack()
{
    AActor* OwnerActor = GetOwner();
    if (!OwnerActor) return;

    FVector ActorLocation = OwnerActor->GetActorLocation();
    FRotator ActorRotation = OwnerActor->GetActorRotation();

    FRotator LeftRotator = FRotator(0.f, ActorRotation.Yaw - 20.f, 0.f);
    FVector LeftVector = LeftRotator.RotateVector(FVector::ForwardVector) * 90.f + ActorLocation;

    FRotator RightRotator = FRotator(0.f, ActorRotation.Yaw + 20.f, 0.f);
    FVector RightVector = RightRotator.RotateVector(FVector::ForwardVector) * 90.f + ActorLocation;

    FVector HalfSize = FVector(20.f, 20.f, 10.f);

    TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
    ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel1));

    TArray<AActor*> ActorsToIgnore;
    ActorsToIgnore.Add(OwnerActor);

    TArray<FHitResult> OutHits;

    bool bHit = UKismetSystemLibrary::BoxTraceMultiForObjects(
        this->GetWorld(),
        LeftVector,
        RightVector,
        HalfSize,
        FRotator::ZeroRotator,
        ObjectTypes,
        false,
        ActorsToIgnore,
        EDrawDebugTrace::ForDuration,
        OutHits,
        true,
        FLinearColor::Red,
        FLinearColor::Green,
        0.2f
    );

    APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(OwnerActor);
    if (!PlayerCharacter) return;

    UPlayerStatusComponent* PlayerStatusComponent = PlayerCharacter->GetPlayerStatusComponent();
    const int32 Damage = PlayerStatusComponent->GetDamage();

    if (bHit)
    {
        for (const FHitResult& Hit : OutHits)
        {
            AActor* HitActor = Hit.GetActor();
            if (!HitActor || DamagedActors.Contains(HitActor)) continue;

            UE_LOG(LogTemp, Warning, TEXT("Hit Actor: %s"), *HitActor->GetName());

            if (ACharacterMonster* CM = Cast<ACharacterMonster>(HitActor))
            {
                FDamageEvent DamageEvent;
                CM->IMonsterInterface::TakeDamage(Damage, DamageEvent, PlayerCharacter->GetController(), PlayerCharacter);
                DamagedActors.Add(HitActor); // 중복 방지용
            }
            else if (APawnMonster* PM = Cast<APawnMonster>(HitActor))
            {
                FDamageEvent DamageEvent;
                PM->IMonsterInterface::TakeDamage(Damage, DamageEvent, PlayerCharacter->GetController(), PlayerCharacter);
                DamagedActors.Add(HitActor); // 중복 방지용
            }
        }
    }
}

