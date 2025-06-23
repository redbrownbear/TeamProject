// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Weapon/WeaponSpear.h"
#include "Actors/Monster/CharacterMonster.h"
#include "Actors/Monster/PawnMonster.h"
#include "Actors/Character/PlayerCharacter.h"

#include "Engine/DamageEvents.h"
AWeaponSpear::AWeaponSpear()
{
	{
		ConstructorHelpers::FObjectFinder<USpearAttackDataAsset> Asset(TEXT("/Script/TeamProject.SpearAttackDataAsset'/Game/Resources/Player/Assets/SpearAttackDataAsset.SpearAttackDataAsset'"));

		if (Asset.Object)
		{
			DataAsset = Asset.Object;
		}
	}
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	{
		ConstructorHelpers::FObjectFinder<UStaticMesh> Asset(TEXT("/Script/Engine.StaticMesh'/Game/Resources/Weapon/SpearStaticMesh/Spear_001.Spear_001'"));

		if (Asset.Object)
		{
			StaticMeshComponent->SetStaticMesh(Asset.Object);
		}
	}
    WeaponType = EWeapon_Type::Spear;
}

void AWeaponSpear::BeginPlay()
{
	Super::BeginPlay();

	EquipMontage = DataAsset->Equip_Montage;
	UnEquipMontage = DataAsset->UnEquip_Montage;
}

void AWeaponSpear::LeftClickAction()
{
    AActor* OwnerActor = GetOwner();
    if (!OwnerActor) return;

    float Distance = 100.f;
    FVector WeaponLocation = GetActorLocation();

    FVector WeaponAttackDistance = WeaponLocation + Distance * GetActorForwardVector();

    FVector HalfSize = FVector(20, 20, 20);


    TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
    ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel1));

    TArray<AActor*> ActorsToIgnore;
    ActorsToIgnore.Add(OwnerActor);

    TArray<FHitResult> OutHits;

    bool bHit = UKismetSystemLibrary::BoxTraceMultiForObjects(
        this->GetWorld(),
        WeaponLocation,
        WeaponAttackDistance,
        HalfSize,
        FRotator::ZeroRotator,
        ObjectTypes,
        false,
        ActorsToIgnore,
        EDrawDebugTrace::None,
        OutHits,
        true,
        FLinearColor::Red,
        FLinearColor::Green,
        0.2f
    );

    mCombo = (mCombo < MaxCombo) ? mCombo : 0;
	APlayerCharacter* Player_C = Cast<APlayerCharacter>(GetOwner());
    
    if (bHit)
    {
        Player_C->GetMesh()->GetAnimInstance()->Montage_Play(DataAsset->Attack_Normal[mCombo]);
    }
    else {
        Player_C->GetMesh()->GetAnimInstance()->Montage_Play(DataAsset->Attack_Advance[mCombo]);
    }
    mCombo += 1;
    Cast<UPlayerMovementComponent>(Player_C->GetCharacterMovement())->SetMoveState(EMove_State::None);
}

void AWeaponSpear::RightClickAction()
{
}

void AWeaponSpear::Attack()
{
    AActor* OwnerActor = GetOwner();
    if (!OwnerActor) return;

    float Distance = 100.f;
    FVector WeaponLocation = GetActorLocation();
    
    FVector WeaponAttackDistance = WeaponLocation + Distance * GetActorForwardVector();

    FVector HalfSize = FVector(20, 20, 20);


    TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
    ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel1));

    TArray<AActor*> ActorsToIgnore;
    ActorsToIgnore.Add(OwnerActor);

    TArray<FHitResult> OutHits;

    bool bHit = UKismetSystemLibrary::BoxTraceMultiForObjects(
        this->GetWorld(),
        WeaponLocation,
        WeaponAttackDistance,
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

void AWeaponSpear::EmptyDamagedActors()
{
    DamagedActors.Empty();
}


