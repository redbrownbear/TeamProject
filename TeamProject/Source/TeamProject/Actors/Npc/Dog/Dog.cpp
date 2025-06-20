// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Npc/Dog/Dog.h"
#include "Actors/Character/PlayerCharacter.h"

#include "Components/SphereComponent.h"

#include "Components/MovementComponent/AdvancedFloatingPawnMovement.h"
#include "GameFramework/PC_InGame.h"

#include "UI/HUD/MainHUD.h"

// Sets default values
ADog::ADog()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	//AIControllerClass = ANpcController::StaticClass();

	BodyMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Body"));

	SetRootComponent(CollisionComponent);
	BodyMeshComponent->SetupAttachment(RootComponent);

	// Movement
	MovementComponent = CreateDefaultSubobject<UAdvancedFloatingPawnMovement>(TEXT("MovementComponent"));
	MovementComponent->UpdatedComponent = CollisionComponent; // 충돌 기준 컴포넌트 설정

	// Collision Overlap Event Binding
	if (CollisionComponent)
	{
		CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ADog::OnBeginOverlapWithPlayer);
		CollisionComponent->OnComponentEndOverlap.AddDynamic(this, &ADog::OnEndOverlapWithPlayer);
	}

	// Collision Setting
	BodyMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BodyMeshComponent->SetCollisionProfileName(TEXT("Pawn"));
}

// Called when the game starts or when spawned
void ADog::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADog::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ADog::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ADog::OnBeginOverlapWithPlayer(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor))
	{
		if (APC_InGame* PC = Cast<APC_InGame>(Player->GetController()))
		{
			//PC->Npc = this;
			bPlayerInRange = true;

			if (AMainHUD* HUD = Cast<AMainHUD>(PC->GetHUD()))
			{
				HUD->ShowInteractWidget(bPlayerInRange); // Create Interact UI
				//HUD->ShowInteractName(bPlayerInRange, NpcData->NPCName);
			}
		}
	}
}

void ADog::OnEndOverlapWithPlayer(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor))
	{
		if (APC_InGame* PC = Cast<APC_InGame>(Player->GetController()))
		{
			//PC->Npc = nullptr;
			bPlayerInRange = false;

			if (AMainHUD* HUD = Cast<AMainHUD>(PC->GetHUD()))
			{
				HUD->ShowInteractWidget(bPlayerInRange);
				//HUD->ShowInteractName(bPlayerInRange, NpcData->NPCName);
			}
		}
	}
}

void ADog::PlayMontage(EDogMontage _InEnum, bool bIsLoop)
{
	UAnimInstance* AnimInstance = BodyMeshComponent->GetAnimInstance();

	/*if (!NpcData) return;

	UAnimMontage* TempAnimMontage = nullptr;
	switch (_InEnum)
	{
	case ENpcMontage::IDLE:
		TempAnimMontage = NpcData->IdleMontage;
		break;
	case ENpcMontage::SIT:
		TempAnimMontage = NpcData->SitMontage;
		break;
	case ENpcMontage::STAND:
		TempAnimMontage = NpcData->StandMontage;
		break;
	case ENpcMontage::WALK:
		TempAnimMontage = NpcData->WalkMontage;
		break;
	case ENpcMontage::RUN:
		TempAnimMontage = NpcData->RunMontage;
		break;
	case ENpcMontage::TALK:
		TempAnimMontage = NpcData->TalkMontage;
		break;
	case ENpcMontage::HIDE:
		TempAnimMontage = NpcData->HideMontage;
		break;
	case ENpcMontage::SELL:
		TempAnimMontage = NpcData->SellMontage;
		break;
	default:
		check(false);
		break;
	}*/

	/*if (TempAnimMontage && !AnimInstance->Montage_IsPlaying(TempAnimMontage))
	{
		if (bIsLoop)
		{
			AnimInstance->Montage_Play(TempAnimMontage, 1.0f, EMontagePlayReturnType::MontageLength, 0.0f, true);
		}
		else
		{
			AnimInstance->Montage_Play(TempAnimMontage);
		}
	}*/
}

bool ADog::IsMontage(EDogMontage _InEnum)
{
	UAnimMontage* TempAnimMontage = nullptr;

	/*switch (_InEnum)
	{
	case ENpcMontage::IDLE:
		TempAnimMontage = NpcData->IdleMontage;
		break;
	case ENpcMontage::SIT:
		TempAnimMontage = NpcData->SitMontage;
		break;
	case ENpcMontage::STAND:
		TempAnimMontage = NpcData->StandMontage;
		break;
	case ENpcMontage::WALK:
		TempAnimMontage = NpcData->WalkMontage;
		break;
	case ENpcMontage::RUN:
		TempAnimMontage = NpcData->RunMontage;
		break;
	case ENpcMontage::TALK:
		TempAnimMontage = NpcData->TalkMontage;
		break;
	case ENpcMontage::HIDE:
		TempAnimMontage = NpcData->HideMontage;
		break;
	case ENpcMontage::SELL:
		TempAnimMontage = NpcData->SellMontage;
		break;
	case ENpcMontage::END:
		TempAnimMontage = nullptr;
		break;
	default:
		break;
	}*/

	return TempAnimMontage ? true : false;
}

bool ADog::IsPlayingMontage(EDogMontage _InEnum)
{
	UAnimInstance* AnimInstance = BodyMeshComponent->GetAnimInstance();

	UAnimMontage* TempAnimMontage = nullptr;

	/*switch (_InEnum)
	{
	case ENpcMontage::IDLE:
		TempAnimMontage = NpcData->IdleMontage;
		break;
	case ENpcMontage::SIT:
		TempAnimMontage = NpcData->SitMontage;
		break;
	case ENpcMontage::STAND:
		TempAnimMontage = NpcData->StandMontage;
		break;
	case ENpcMontage::WALK:
		TempAnimMontage = NpcData->WalkMontage;
		break;
	case ENpcMontage::RUN:
		TempAnimMontage = NpcData->RunMontage;
		break;
	case ENpcMontage::TALK:
		TempAnimMontage = NpcData->TalkMontage;
		break;
	case ENpcMontage::HIDE:
		TempAnimMontage = NpcData->HideMontage;
		break;
	case ENpcMontage::SELL:
		TempAnimMontage = NpcData->SellMontage;
		break;
	case ENpcMontage::END:
		TempAnimMontage = nullptr;
		break;
	default:
		break;
	}*/

	return AnimInstance->Montage_IsPlaying(nullptr);
}

