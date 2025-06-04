#include "NpcFSMComponent.h"
#include "Actors/Npc/Npc.h"
#include "Actors/Controller/Npc/NpcController.h"
#include "Actors/Character/PlayerCharacter.h"
#include "GameFramework/PC_InGame.h"

#include "Navigation/PathFollowingComponent.h"
#include "Kismet/GameplayStatics.h"

#include "Components/ConversationComponent/ConversationManagerComponent.h"

#include "Actors/TargetPoint/HidePoint.h"

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
	if (Controller && Owner->GetConversationManager())
	{
		UConversationManagerComponent* ConversationManager = Owner->GetConversationManager();

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

	Owner->GetConversationManager()->SetEndTalked(false);

	switch (NewState)
	{
	case ENpcState::Idle:
		Owner->PlayMontage(ENpcMontage::IDLE);
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
		if (Owner->GetIsHide())
		{
			Owner->PlayMontage(ENpcMontage::STAND);
		}
		// @TODO Play Sequence
		//PlayInterectSequence();
		Owner->PlayMontage(ENpcMontage::TALK);
		Owner->GetConversationManager()->StartConversation(Owner, Player);
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

}

void UNpcFSMComponent::UpdateIdle(float DeltaTime)
{
	if (eCurrentState != ENpcState::Idle)
	{
		UE_LOG(LogTemp, Error, TEXT("eCurrentState is Not 'ENpcState::Idle'"));
		return;
	}
}

void UNpcFSMComponent::UpdateWalk(float DeltaTime)
{
	if (eCurrentState != ENpcState::Walk)
	{
		UE_LOG(LogTemp, Error, TEXT("eCurrentState is Not 'ENpcState::Walk'"));
		return;
	}
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

}

void UNpcFSMComponent::UpdateHide(float DeltaTime)
{
	if (eCurrentState != ENpcState::Hide)
	{
		UE_LOG(LogTemp, Error, TEXT("eCurrentState is Not 'ENpcState::Hide'"));
		return;
	}
}

void UNpcFSMComponent::UpdateSell(float DeltaTime)
{
	if (eCurrentState != ENpcState::Sell)
	{
		UE_LOG(LogTemp, Error, TEXT("eCurrentState is Not 'ENpcState::Sell'"));
		return;
	}
}

void UNpcFSMComponent::UpdateEnd(float DeltaTime)
{
	if (eCurrentState != ENpcState::End)
	{
		UE_LOG(LogTemp, Error, TEXT("eCurrentState is Not 'ENpcState::End'"));
		return;
	}
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
		UE_LOG(LogTemp, Warning, TEXT("HidePoints is nullptr"));
		return;
	}

	const int32 Index = FMath::RandRange(0, HidePoints.Num() - 1);
	AHidePoint* Target = HidePoints[Index];

	if (Target && Owner)
	{
		//UE_LOG(LogTemp, Log, TEXT("Furiko Moves: %s, Location: %s"), *Target->GetName(), *Target->GetActorLocation().ToString());

		SetHideLocation(Target->GetActorLocation());

		// <푸리코와 놀자!> 퀘스트 UI 생성할까 말까 윤호오빠랑 얘기해보기
	}
}

void UNpcFSMComponent::SetHideLocation(FVector InLocation)
{
	if (!Owner) return;
	
	// 이동 방해 차단
	if (Owner->GetController()) Owner->GetController()->StopMovement();

	// 순간이동
	Owner->SetActorLocation(InLocation, false, nullptr, ETeleportType::TeleportPhysics);
	Owner->SetIsHide(true);

	// 위치 확인
	//UE_LOG(LogTemp, Warning, TEXT("SetHideLocation 완료. Owner 위치: %s"), *Owner->GetActorLocation().ToString());

	if (Owner->GetController()) Owner->GetController()->StopMovement();

	// ?쒓컙?대룞
	Owner->SetActorLocation(InLocation, false, nullptr, ETeleportType::TeleportPhysics);
	Owner->SetIsHide(true);

}

void UNpcFSMComponent::PlayInterectSequence()
{
	// Talk 시 캐릭터 위치만 고정되게 조정한 뒤 그 다음에 생성해야 할 듯
	
	/*if (Player)
	{
		FVector PlayerLocation = Player->GetActorLocation();
		FVector NpcLocation = Owner->GetActorLocation();
		SmoothRotateActorToDirection(Owner, PlayerLocation, DeltaTime);
		SmoothRotateActorToDirection(Player, NpcLocation, DeltaTime);
	}*/
}
