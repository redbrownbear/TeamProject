// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/AttackComponent/SwordComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Actors/Character/PlayerCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/PawnMovementComponent.h"

// Sets default values for this component's properties
USwordComponent::USwordComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

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


	// ...
}


// Called when the game starts
void USwordComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void USwordComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void USwordComponent::SetAttackBox()
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
        ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));

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
    GetWorld()->GetTimerManager().SetTimer(
        ComboTimerHandle,
        this,
        &ThisClass::SetCanAttack,
        0.5f,
        false
    );

    GetWorld()->GetTimerManager().SetTimer(
        MoveTimerHandle,
        this,
        &ThisClass::SetCanMove,
        1.5f,
        false
    );
}

void USwordComponent::SetCanAttack()
{
    bCanAttack = true;
}

void USwordComponent::SetCanMove()
{
    AActor* ActorPlayer = GetOwner();
    ACharacter* Character = Cast<ACharacter>(ActorPlayer);

    Character->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
}



