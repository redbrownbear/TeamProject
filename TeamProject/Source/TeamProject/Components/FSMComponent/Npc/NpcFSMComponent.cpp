#include "NpcFSMComponent.h"
#include "Actors/Npc/Npc.h"
#include "Actors/Controller/Npc/NpcController.h"
#include "Actors/Character/PlayerCharacter.h"
#include "GameFramework/PC_InGame.h"

#include "Navigation/PathFollowingComponent.h"
#include "Kismet/GameplayStatics.h"

#include "Components/ConversationComponent/ConversationManagerComponent.h"

#include "Actors/HidePoint/HidePoint.h"

UNpcFSMComponent::UNpcFSMComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UNpcFSMComponent::BeginPlay()
{
	Super::BeginPlay();
		
	if (!Owner)
	{
		Controller = Cast<ANpcController>(GetOwner());
		if (Controller)
		{
			Owner = Cast<ANpc>(Controller->GetPawn());
		}
	}

	if (!Owner)
	{
		UE_LOG(LogTemp, Error, TEXT("UNpcFSMComponent::BeginPlay // Owner (ANpc) is null!"));
		return;
	}

	if (APlayerCharacter* PlayerChar = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0)))
	{
		Player = PlayerChar;
	}

	APC_InGame* PC = Cast<APC_InGame>(Player->GetController());
	if (PC)
	{
		PC->SetNpc(Owner);
	}

	Controller = Cast<ANpcController>(Owner->GetController());
	if (Controller && Controller->GetConversationManager())
	{
		UConversationManagerComponent* ConversationManager = Controller->GetConversationManager();

		if (!ConversationManager)
		{
			UE_LOG(LogTemp, Error, TEXT("UNpcFSMComponent::BeginPlay // ConversationManager is null (from NPC)!"));
		}
	}

	TArray<AActor*> Found;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AHidePoint::StaticClass(), Found);

	for (AActor* Actor : Found)
	{
		if (AHidePoint* HidePoint = Cast<AHidePoint>(Actor))
		{
			HidePoints.Add(HidePoint);
		}
	}

	UE_LOG(LogTemp, Log, TEXT("HidePoints 자동 수집 완료: %d개"), HidePoints.Num());
}

void UNpcFSMComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	HandleState(DeltaTime);
}

void UNpcFSMComponent::HandleState(float DeltaTime)
{
	if (!Owner)
	{
		UE_LOG(LogTemp, Error, TEXT("UNpcFSMComponent::HandleState // No Owner"));
		check(false);
		return;
	}

	switch (eCurrentState)
	{
	case ENpcState::Idle:
		UpdateIdle(DeltaTime);
		break;
	case ENpcState::Sit:
		UpdateSit(DeltaTime);
		break;
	case ENpcState::Stand:
		UpdateStand(DeltaTime);
		break;
	case ENpcState::Walk:
		UpdateWalk(DeltaTime);
		break;
	case ENpcState::Run:
		UpdateRun(DeltaTime);
		break;
	case ENpcState::Talk:
		UpdateTalk(DeltaTime);
		break;
	case ENpcState::Hide:
		UpdateHide(DeltaTime);
		break;
	case ENpcState::Sell:
		UpdateSell(DeltaTime);
		break;
	case ENpcState::End:
		UpdateEnd(DeltaTime);
		break;
	default:
		break;
	}
	
}

void UNpcFSMComponent::ChangeState(ENpcState NewState)
{	
	if (eCurrentState == NewState) { return; }

	eCurrentState = NewState;

	switch (NewState)
	{
	case ENpcState::Idle:
		Owner->PlayMontage(ENpcMontage::IDLE);
		break;
	case ENpcState::Sit:
		Owner->PlayMontage(ENpcMontage::SIT);
		break;
	case ENpcState::Stand:
		Owner->PlayMontage(ENpcMontage::STAND);
		break;
	case ENpcState::Walk:
		Owner->SetSpeedWalk();
		Owner->PlayMontage(ENpcMontage::WALK);
		break;
	case ENpcState::Run:
		Owner->SetSpeedRun();
		Owner->PlayMontage(ENpcMontage::RUN);
		break;
	case ENpcState::Talk:
		Controller->GetConversationManager()->StartConversation(Owner, Player);
		break;	
	case ENpcState::Hide:
		Owner->PlayMontage(ENpcMontage::HIDE);
		HideFuriko();
		break;
	case ENpcState::Sell:
		Owner->PlayMontage(ENpcMontage::SELL);
		break;
	case ENpcState::End:
		Owner->PlayMontage(ENpcMontage::END);
		break;
	default:
		break;
	}

	Controller->GetConversationManager()->SetStateChanged(false);

}

void UNpcFSMComponent::UpdateIdle(float DeltaTime)
{
	if (eCurrentState != ENpcState::Idle)
	{
		UE_LOG(LogTemp, Error, TEXT("eCurrentState is Not 'ENpcState::Idle'"));
		return;
	}
}

void UNpcFSMComponent::UpdateSit(float DeltaTime)
{
}

void UNpcFSMComponent::UpdateStand(float DeltaTime)
{
}

void UNpcFSMComponent::UpdateWalk(float DeltaTime)
{
}

void UNpcFSMComponent::UpdateRun(float DeltaTime)
{	
	if (eCurrentState != ENpcState::Run)
	{
		UE_LOG(LogTemp, Error, TEXT("eCurrentState is Not 'ENpcState::Run'"));
		return;
	}
	
}

void UNpcFSMComponent::UpdateTalk(float DeltaTime)
{
	if (eCurrentState != ENpcState::Talk)
	{
		UE_LOG(LogTemp, Error, TEXT("eCurrentState is Not 'ENpcState::Talk'"));
		return;
	}

	if (Player)
	{	
		FVector PlayerLocation = Player->GetActorLocation();
		FVector NpcLocation = Owner->GetActorLocation();
		SmoothRotateActorToDirection(Owner, PlayerLocation, DeltaTime);
		SmoothRotateActorToDirection(Player, NpcLocation, DeltaTime);		
	}			

}

void UNpcFSMComponent::UpdateHide(float DeltaTime)
{

}

void UNpcFSMComponent::UpdateSell(float DeltaTime)
{
}

void UNpcFSMComponent::UpdateEnd(float DeltaTime)
{
}

void UNpcFSMComponent::MoveToLocation(const FVector& InLocation)
{
	ANpcController* NpcController = Cast<ANpcController>(Owner->GetController());
	if (NpcController)
	{
		FAIMoveRequest MoveRequest;
		MoveRequest.SetGoalLocation(InLocation);
		MoveRequest.SetAcceptanceRadius(AcceptanceRadius);

		FNavPathSharedPtr NavPath;
		NpcController->MoveTo(MoveRequest, &NavPath);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("NpcFSMComponent::MoveToLocation // No NpcController"));
		check(false);
	}
}

void UNpcFSMComponent::HideFuriko()
{
	if (HidePoints.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("HidePoints 배열이 비어있습니다."));
		return;
	}

	// 랜덤 인덱스 선택
	const int32 Index = FMath::RandRange(0, HidePoints.Num() - 1);
	AHidePoint* Target = HidePoints[Index];

	if (Target && Owner)
	{
		SetHideLocation(Target->GetActorLocation());

		// <푸리코와 놀자!> 퀘스트 UI 생성할까 말까 윤호오빠랑 얘기해보기

		UE_LOG(LogTemp, Log, TEXT("Furiko가 HidePoint %s 로 순간이동했습니다."), *Target->GetName());
	}
}

void UNpcFSMComponent::SetHideLocation(FVector InLocation)
{
	if (!Owner) return;

	// 순간 이동
	Owner->SetActorLocation(InLocation, false, nullptr, ETeleportType::TeleportPhysics);

	UE_LOG(LogTemp, Log, TEXT("Furiko가 HidePoint로 이동했습니다: %s"), *InLocation.ToString());
}
