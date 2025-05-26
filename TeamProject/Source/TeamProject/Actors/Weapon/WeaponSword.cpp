// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Weapon/WeaponSword.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Actors/Character/PlayerCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/PawnMovementComponent.h"

AWeaponSword::AWeaponSword()
{
    {

        PrimaryActorTick.bCanEverTick = true;

        DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
        RootComponent = DefaultSceneRoot;


        ConstructorHelpers::FObjectFinder<USkeletalMesh> Asset(TEXT("/Script/Engine.SkeletalMesh'/Game/Resources/Player/Sword/Weapon_Sword_001.Weapon_Sword_001'"));

        SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
        SkeletalMeshComponent->SetupAttachment(RootComponent);
        SkeletalMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        SkeletalMeshComponent->SetSkeletalMesh(Asset.Object);
    }

    {
        ConstructorHelpers::FObjectFinder<UAnimMontage> Asset(TEXT("/Script/Engine.AnimMontage'/Game/Resources/Player/Armor/Animation/Sword_Attack/Sword_Attack_Charge_L_Montage.Sword_Attack_Charge_L_Montage'"));

        if (Asset.Object)
        {
            Arr_Sword_Attack_MTG.Add(Asset.Object);
            MaxComboIndex += 1;
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("No Anim_Montage"));
        }
    }

    {
        ConstructorHelpers::FObjectFinder<UAnimMontage> Asset(TEXT("/Script/Engine.AnimMontage'/Game/Resources/Player/Armor/Animation/Sword_Attack/Sword_Attack_Charge_Montage.Sword_Attack_Charge_Montage'"));

        if (Asset.Object)
        {
            Arr_Sword_Attack_MTG.Add(Asset.Object);
            MaxComboIndex += 1;
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("No Anim_Montage"));
        }
    }

    {
        ConstructorHelpers::FObjectFinder<UAnimMontage> Asset(TEXT("/Script/Engine.AnimMontage'/Game/Resources/Player/Sword/Animation/Equip_Sword_On_Natural_Montage.Equip_Sword_On_Natural_Montage'"));

        if (Asset.Object)
        {
            EquipMontage = Asset.Object;
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("No Anim_Montage"));
        }
    }

    {
        ConstructorHelpers::FObjectFinder<UAnimMontage> Asset(TEXT("/Script/Engine.AnimMontage'/Game/Resources/Player/Sword/Animation/Equip_Sword_Off_Montage.Equip_Sword_Off_Montage'"));

        if (Asset.Object)
        {
            UnEquipMontage = Asset.Object;
            
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("No Anim_Montage"));
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
        CurrentComboIndex += 1;
        CurrentComboIndex = (MaxComboIndex <= CurrentComboIndex) ? 0 : CurrentComboIndex;

        Player_C->GetMesh()->GetAnimInstance()->Montage_Play(Arr_Sword_Attack_MTG[CurrentComboIndex]);
        Player_C->GetCharacterMovement()->SetMovementMode(MOVE_None);

    }




    {

        FVector ActorLocation = OwnerActor->GetActorLocation();
        FRotator ActorRotation = OwnerActor->GetActorRotation();

        // 현재 전방 벡터
        FVector ForwardVector = ActorRotation.Vector();

        // Yaw -15도 회전
        FRotator LeftRotator = FRotator(0.f, ActorRotation.Yaw - 15.f, 0.f);
        FVector LeftVector = LeftRotator.RotateVector(FVector::ForwardVector) * 70.f + ActorLocation;

        // Yaw +15도 회전
        FRotator RightRotator = FRotator(0.f, ActorRotation.Yaw + 15.f, 0.f);
        FVector RightVector = RightRotator.RotateVector(FVector::ForwardVector) * 70.f + ActorLocation;

        // 변수에 대입
        FVector Left = LeftVector;
        FVector Right = RightVector;

        // 박스 설정
        FVector HalfSize = FVector(10.f, 10.f, 10.f);
        FRotator Orientation = FRotator::ZeroRotator;

        // 감지할 오브젝트 타입 (Pawn)
        TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
        ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel1));

        // 무시할 액터
        TArray<AActor*> ActorsToIgnore;
        ActorsToIgnore.Add(OwnerActor);

        // 결과 저장용
        TArray<FHitResult> OutHits;

        // 박스 트레이스 실행
        bool bHit = UKismetSystemLibrary::BoxTraceMultiForObjects(
            this->GetWorld(),        // WorldContextObject
            Left,                   // Start
            Right,                  // End
            FVector(10.f, 10.f, 10.f), // HalfSize
            FRotator::ZeroRotator,  // Orientation
            ObjectTypes,            // ObjectTypes
            false,                  // bTraceComplex
            ActorsToIgnore,         // ActorsToIgnore
            EDrawDebugTrace::ForDuration, // DrawDebugType
            OutHits,                // OutHits (여기 위치 중요!)
            true,                   // bIgnoreSelf
            FLinearColor::Red,      // TraceColor
            FLinearColor::Green,    // TraceHitColor
            0.2f                    // DrawTime
        );

        // 결과 출력 감지된 Actor UE_LOG 에 출력
        if (bHit)
        {
            for (const FHitResult& Hit : OutHits)
            {
                if (AActor* HitActor = Hit.GetActor())
                {
                    UE_LOG(LogTemp, Warning, TEXT("Hit Actor: %s"), *HitActor->GetName());
                }
            }
        }
    }
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


