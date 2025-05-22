#include "Npc.h"
#include "Actors/Controller/Npc/NpcController.h"

#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/FSMComponent/Npc/NpcFSMComponent.h"

#include "GameFramework/FloatingPawnMovement.h"
#include "Kismet/KismetMathLibrary.h"

#include "Actors/Character/PlayerCharacter.h"
#include "GameFramework/PC_InGame.h"

#include "Components/ConversationComponent/ConversationManagerComponent.h"
#include "SubSystem/UI/QuestDialogueManager.h"

#include "UI/HUD/MainHUD.h"

ANpc::ANpc()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	SetRootComponent(CollisionComponent);
	AIControllerClass = ANpcController::StaticClass();

	BodyMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Body"));
	HeadMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Head"));
	HairMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Hair"));
	NoseMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Nose"));

	SetRootComponent(CollisionComponent);
	BodyMeshComponent->SetupAttachment(RootComponent);
	HeadMeshComponent->SetupAttachment(BodyMeshComponent, TEXT("Head"));
	HairMeshComponent->SetupAttachment(HeadMeshComponent, TEXT("Hair"));
	NoseMeshComponent->SetupAttachment(HeadMeshComponent, TEXT("Nose"));

	// Movement
	MovementComponent = CreateDefaultSubobject<UAdvancedFloatingPawnMovement>(TEXT("MovementComponent"));
	MovementComponent->UpdatedComponent = CollisionComponent; // 충돌 기준 컴포넌트 설정

	// Collision Overlap Event Binding
	if (CollisionComponent)
	{
		CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ANpc::OnBeginOverlapWithPlayer);
		CollisionComponent->OnComponentEndOverlap.AddDynamic(this, &ANpc::OnEndOverlapWithPlayer);
	}

	// Collision Setting
	BodyMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BodyMeshComponent->SetCollisionProfileName(TEXT("Pawn"));

}

void ANpc::BeginPlay()
{
	Super::BeginPlay();
	
	UE_LOG(LogTemp, Warning, TEXT("HeadMeshComponent: %s"), *GetNameSafe(HeadMeshComponent));
	UE_LOG(LogTemp, Warning, TEXT("HeadMeshAsset: %s"), *GetNameSafe(HeadMeshAsset));

	UE_LOG(LogTemp, Warning, TEXT("HeadMeshComponent SkeletalMesh: %s"), *GetNameSafe(HeadMeshComponent->SkeletalMesh));

	SetData(DataTableRowHandle);
}

void ANpc::OnConstruction(const FTransform& Transform)
{
	SetData(DataTableRowHandle);
}

// Called every frame
void ANpc::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

UNpcFSMComponent* ANpc::GetFSMComponent() const
{
	if (ANpcController* NpcController = Cast<ANpcController>(GetController()))
	{
		if (UNpcFSMComponent* FSMComponent = Cast<UNpcFSMComponent>(NpcController->GetComponentByClass(UNpcFSMComponent::StaticClass())))
		{
			return FSMComponent;
		}
	}

	return nullptr;

}

void ANpc::OnBeginOverlapWithPlayer(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor))
	{
		if (APC_InGame* PC = Cast<APC_InGame>(Player->GetController()))
		{
			PC->Npc = this;
			bPlayerInRange = true;

			if (AMainHUD* HUD = Cast<AMainHUD>(PC->GetHUD()))
			{
				HUD->ShowInteractWidget(bPlayerInRange);
				HUD->ShowInteractName(bPlayerInRange, NpcData->NPCName);
			}
		}
		// Create Interact UI
	}
}

void ANpc::OnEndOverlapWithPlayer(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor))
	{
		if (APC_InGame* PC = Cast<APC_InGame>(Player->GetController()))
		{
			PC->Npc = nullptr;
			bPlayerInRange = false;
      
			if (AMainHUD* HUD = Cast<AMainHUD>(PC->GetHUD()))
			{
				HUD->ShowInteractWidget(bPlayerInRange);
				HUD->ShowInteractName(bPlayerInRange, NpcData->NPCName);
			}
		}
	
		// Delete Interact UI
	}
}

void ANpc::SetSpeedWalk()
{
	MovementComponent->MaxSpeed = NpcData->WalkMovementMaxSpeed;
}

void ANpc::SetSpeedRun()
{
	MovementComponent->MaxSpeed = NpcData->RunMovementMaxSpeed;
}

void ANpc::OnTalkKeyPressed()
{
	if (bPlayerInRange && IsValid(NpcFSMComponent))
	{
		NpcFSMComponent->ChangeState(ENpcState::Talk);
		// Delete Interact UI And Create Talk UI		
		// Delete Interact UI 	
	}
}

void ANpc::AttachToSocket()
{
	if (BodyMeshComponent && HeadMeshComponent && BodyMeshComponent->SkeletalMesh)
	{
		if (BodyMeshComponent->DoesSocketExist(TEXT("Head")))
		{
			HeadMeshComponent->AttachToComponent(
				BodyMeshComponent,
				FAttachmentTransformRules::SnapToTargetIncludingScale,
				TEXT("Head"));
		}
	}

	if (HeadMeshComponent && HairMeshComponent && HeadMeshComponent->SkeletalMesh)
	{
		if (HeadMeshComponent->DoesSocketExist(TEXT("Hair")))
		{
			HairMeshComponent->AttachToComponent(
				HeadMeshComponent,
				FAttachmentTransformRules::SnapToTargetIncludingScale,
				TEXT("Hair"));
		}
	}

	if (HeadMeshComponent && NoseMeshComponent && HeadMeshComponent->SkeletalMesh)
	{
		if (HeadMeshComponent->DoesSocketExist(TEXT("Nose")))
		{
			NoseMeshComponent->AttachToComponent(
				HeadMeshComponent,
				FAttachmentTransformRules::SnapToTargetIncludingScale,
				TEXT("Nose"));
		}
	}

	if (HeadMeshComponent && BodyMeshComponent)
		HeadMeshComponent->SetLeaderPoseComponent(BodyMeshComponent);

	if (HairMeshComponent && HeadMeshComponent)
		HairMeshComponent->SetLeaderPoseComponent(HeadMeshComponent);

	if (NoseMeshComponent && HeadMeshComponent)
		NoseMeshComponent->SetLeaderPoseComponent(HeadMeshComponent);	
}

void ANpc::SetData(const FDataTableRowHandle& InDataTableRowHandle)
{
	DataTableRowHandle = InDataTableRowHandle;
	if (DataTableRowHandle.IsNull()) { return; }
	FNpcCharacterTableRow* Data = DataTableRowHandle.GetRow<FNpcCharacterTableRow>(DataTableRowHandle.RowName.ToString());
	if (!Data) { return; }
	NpcData = Data;

	if (CollisionComponent)
	{
		CollisionComponent->SetSphereRadius(NpcData->CollisionSphereRadius);
		CollisionComponent->SetCollisionProfileName(CollisionProfileName::Monster);
		CollisionComponent->bHiddenInGame = COLLISION_HIDDEN_IN_GAME;
		CollisionComponent->RegisterComponent();
	}

	BodyMeshComponent->SetSkeletalMesh(NpcData->SkeletalMesh);
	BodyMeshComponent->SetAnimClass(NpcData->AnimClass);
	BodyMeshComponent->SetRelativeScale3D(NpcData->MeshTransform.GetScale3D());
	
	MovementComponent->MaxSpeed = NpcData->WalkMovementMaxSpeed;

	AIControllerClass = NpcData->NpcControllerClass;

	if (HeadMeshAsset && HeadMeshComponent)
	{
		HeadMeshComponent->SetSkeletalMesh(HeadMeshAsset);
	}
	if (HairMeshAsset && HairMeshComponent)
	{
		HairMeshComponent->SetSkeletalMesh(HairMeshAsset);
	}
	if (NoseMeshAsset && NoseMeshComponent)
	{
		NoseMeshComponent->SetSkeletalMesh(NoseMeshAsset);
	}

	AttachToSocket();
}

void ANpc::PlayMontage(ENpcMontage _InEnum, bool bIsLoop)
{
	UAnimInstance* AnimInstance = BodyMeshComponent->GetAnimInstance();

	if (!NpcData) return;

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
	}

	if (TempAnimMontage && !AnimInstance->Montage_IsPlaying(TempAnimMontage))
	{
		if (bIsLoop)
		{
			AnimInstance->Montage_Play(TempAnimMontage, 1.0f, EMontagePlayReturnType::MontageLength, 0.0f, true);
		}
		else
		{
			AnimInstance->Montage_Play(TempAnimMontage);
		}
	}
}

bool ANpc::IsMontage(ENpcMontage _InEnum)
{
	if (!NpcData) return false; 

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
	case ENpcMontage::END:
		TempAnimMontage = nullptr;
		break;
	default:
		break;
	}

	return TempAnimMontage ? true : false;
}

bool ANpc::IsPlayingMontage(ENpcMontage _InEnum)
{
	if (!NpcData) return false;
	UAnimInstance* AnimInstance = BodyMeshComponent->GetAnimInstance();

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
	case ENpcMontage::END:
		TempAnimMontage = nullptr;
		break;
	default:
		break;
	}

	return AnimInstance->Montage_IsPlaying(nullptr);
}
