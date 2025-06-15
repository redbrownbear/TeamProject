// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/FSMComponent/Monster/HinoxFSMComponent.h"
#include "Actors/Monster/CharacterMonster.h"
#include "Actors/Character/PlayerCharacter.h"

#include "Components/StatusComponent/MonsterStatusComponent/MonsterStatusComponent.h"

#include "GameFramework/PC_InGame.h"

#include "UI/HUD/MainHUD.h"

#include "Data/MonsterTableRow.h"


UHinoxFSMComponent::UHinoxFSMComponent()
{
	eCurrentState = EMonsterState::Idle;
}

void UHinoxFSMComponent::HandleState(float DeltaTime)
{
	if (!CharacterMonster)
	{
		UE_LOG(LogTemp, Error, TEXT("UHinoxFSMComponent::HandleState // No CharacterMonster"));
		check(false);
		return;
	}

	switch (eCurrentState)
	{
	case EMonsterState::Idle:
		UpdateIdle(DeltaTime);
		break;
	case EMonsterState::Alert:
		UpdateAlert(DeltaTime);
		break;
	case EMonsterState::Combat:
		UpdateCombat(DeltaTime);
		break;
	case EMonsterState::Dead:
		UpdateDying(DeltaTime);
		break;
	case EMonsterState::Damage_Eye:
		UpdateDamageEye(DeltaTime);
		break;
	case EMonsterState::Temp:
		UpdateTemp(DeltaTime);
		break;
	case EMonsterState::End:
	default:
		UE_LOG(LogTemp, Error, TEXT("UHinoxFSMComponent::HandleState // Unexpected MonsterState"));
		check(false);
		break;
	}
}

void UHinoxFSMComponent::ChangeState(EMonsterState NewState)
{
	if (eCurrentState == NewState) { return; }
	if (!CharacterMonster)
	{
		UE_LOG(LogTemp, Error, TEXT("UHinoxFSMComponent::ChangeState // No CharacterMonster"));
		return;
	}


	const EMonsterState PrevState = eCurrentState;

	switch (PrevState)
	{
	case EMonsterState::Idle:
		//CharacterMonster->PlayMontage(EMonsterMontage::SLEEP_END);
		break;
	case EMonsterState::Alert:
		break;
	case EMonsterState::Combat:

		//switch (eCombatIndex)
		//{
		//case EHinoxCombatIndex::Foot:
		//	eCombatIndex = EHinoxCombatIndex::HandClap;
		//	break;
		//case EHinoxCombatIndex::HandClap:
		//	eCombatIndex = EHinoxCombatIndex::LeftHand;
		//	break;
		//case EHinoxCombatIndex::LeftHand:
		//	eCombatIndex = EHinoxCombatIndex::Hipdrop;
		//	break;
		//case EHinoxCombatIndex::Hipdrop:
		//	eCombatIndex = EHinoxCombatIndex::Foot;
		//	break;
		//case EHinoxCombatIndex::End:
		//	break;
		//default:
		//	UE_LOG(LogTemp, Error, TEXT("ULynelFSMComponent::ChangeState // Unexpected CombatIndex"));
		//	check(false);
		//	break;
		//}
		break;
	case EMonsterState::Damage_Eye:
		CharacterMonster->PlayMontage(EMonsterMontage::DAMAGE_EYE_END);
		break;
	case EMonsterState::Dead:
		break;
	case EMonsterState::Damage:
		break;
	default:
		UE_LOG(LogTemp, Error, TEXT("ULynelFSMComponent::ChangeState // Unexpected MonsterState"));
		check(false);
		break;
	}


	switch (NewState)
	{
	case EMonsterState::Idle:
		CharacterMonster->PlayMontage(EMonsterMontage::SLEEP_START);
		break;
	case EMonsterState::Alert:
		break;
	case EMonsterState::Combat:
		break;
	case EMonsterState::Damage_Eye:
		DamageEyeElapsedTime = 0.f;
		CharacterMonster->PlayMontage(EMonsterMontage::DAMAGE_EYE_START);
		break;
	case EMonsterState::Dead:
		CharacterMonster->PlayMontage(EMonsterMontage::DEAD);
		break;
	case EMonsterState::Damage:
		return;
		break;
	default:
		UE_LOG(LogTemp, Error, TEXT("UHinoxFSMComponent::ChangeState // Unexpected MonsterState, ChangeState Failed"));
		return;
		break;
	}

	switch (NewState)
	{
	case EMonsterState::Idle:
	case EMonsterState::Alert:
	case EMonsterState::Dead:
		if (UWorld* World = CharacterMonster->GetWorld())
		{
			if (APC_InGame* PC = Cast<APC_InGame>(World->GetFirstPlayerController()))
			{
				if (AMainHUD* HUD = Cast<AMainHUD>(PC->GetHUD()))
				{
					if (UMonsterStatusComponent* StatusComponent = CharacterMonster->GetStatusComponent())
					{
						HUD->ShowBossHpUI(false, StatusComponent->GetCurrentHP(), StatusComponent->GetMaxHP(), CharacterMonster->GetMonsterData()->Name.ToString());
					}
				}
			}
		}
		break;
	case EMonsterState::Combat:
	case EMonsterState::Damage_Eye:
	case EMonsterState::Damage:
		if (UWorld* World = CharacterMonster->GetWorld())
		{
			if (APC_InGame* PC = Cast<APC_InGame>(World->GetFirstPlayerController()))
			{
				if (AMainHUD* HUD = Cast<AMainHUD>(PC->GetHUD()))
				{
					if (UMonsterStatusComponent* StatusComponent = CharacterMonster->GetStatusComponent())
					{
						HUD->ShowBossHpUI(true, StatusComponent->GetCurrentHP(), StatusComponent->GetMaxHP(), CharacterMonster->GetMonsterData()->Name.ToString());
					}
				}
			}
		}
		break;
	default:
		UE_LOG(LogTemp, Error, TEXT("UHinoxFSMComponent::ChangeState // Unexpected MonsterState, ChangeState Failed"));
		return;
		break;
	}

	eCurrentState = NewState;
}

void UHinoxFSMComponent::UpdateIdle(float DeltaTime)
{
	this->StopMove();

	if (Player)
	{

		if (!CharacterMonster->IsPlayingMontage(EMonsterMontage::SLEEP_END))
		{
			CharacterMonster->PlayMontage(EMonsterMontage::SLEEP_END);
		}

		FTimerHandle TimerHandle;
		EMonsterState TargetState = EMonsterState::Combat;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this, TargetState]()
			{
				ChangeState(TargetState);
			}, 0.5f, false);
	}
}

void UHinoxFSMComponent::UpdateAlert(float DeltaTime)
{
	this->StopMove();
}

void UHinoxFSMComponent::UpdateCombat(float DeltaTime)
{
	if (!Player)
	{
		UE_LOG(LogTemp, Warning, TEXT("ULynelFSMComponent::UpdateCombat // No Player"));
		ChangeState(EMonsterState::Idle);
		return;
	}

	const FVector PlayerLocation = Player->GetActorLocation();
	const FVector MonsterLocation = CharacterMonster->GetActorLocation();

	const float fDistance = FVector::Dist(PlayerLocation, MonsterLocation);
	if (fDistance > MONSTER_AISENSECONFIG_SIGHT_LOSESIGHTRADIUS)
	{
		Player = nullptr;
		ChangeState(EMonsterState::Idle);
		return;
	}





	if (CharacterMonster->IsPlayingMontage(EMonsterMontage::END))
	{
		if (CharacterMonster->IsPlayingMontage(EMonsterMontage::THROW_STONE_END))
		{
			SmoothRotateActorToDirection(CharacterMonster, PlayerLocation, DeltaTime, 3.f);
		}
		this->StopMove();
		return;
	}

	const bool bIsNear = FVector::PointsAreNear(MonsterLocation, PlayerLocation, HINOX_NEAR_INSTANCE);

	if (bIsNear)
	{
		if (CharacterMonster)
		{
			if (!CharacterMonster->IsPlayingMontage(EMonsterMontage::END)				// Play no montage
				|| CharacterMonster->IsPlayingMontage(EMonsterMontage::RUN)				
				|| CharacterMonster->IsPlayingMontage(EMonsterMontage::RUN_CURVE_R)
				|| CharacterMonster->IsPlayingMontage(EMonsterMontage::RUN_CURVE_L))
			{
				FVector ToPlayer = PlayerLocation - CharacterMonster->GetActorLocation();
				ToPlayer.Z = 0.0f; // 수평 방향만 비교
				ToPlayer.Normalize();

				FVector Forward = CharacterMonster->GetActorForwardVector();
				Forward.Z = 0.0f;
				Forward.Normalize();

				// 오른쪽 기준 벡터 (몬스터의 오른쪽)
				FVector Right = FVector::CrossProduct(FVector::UpVector, Forward);

				float ForwardDot = FVector::DotProduct(Forward, ToPlayer);
				float RightDot = FVector::DotProduct(Right, ToPlayer);

				if (RightDot > 0.3f)
				{
					//SmoothRotateActorToDirection(CharacterMonster, PlayerLocation, DeltaTime, 10.f);
					CharacterMonster->PlayMontage(EMonsterMontage::TURN_180_R); // 오른쪽
					SmoothRotateActorToDirection(CharacterMonster, PlayerLocation, DeltaTime);
					return;
				}
				else if (RightDot < -0.3f)
				{
					//SmoothRotateActorToDirection(CharacterMonster, PlayerLocation, DeltaTime, 10.f);
					CharacterMonster->PlayMontage(EMonsterMontage::TURN_180_L); // 왼쪽
					SmoothRotateActorToDirection(CharacterMonster, PlayerLocation, DeltaTime);
					return;
				}



				switch (eCombatIndex)
				{
				case EHinoxCombatIndex::Foot:
					if (CharacterMonster)
					{
						CharacterMonster->PlayMontage(EMonsterMontage::FOOT);
					}
					else
					{
						UE_LOG(LogTemp, Error, TEXT("ULynelFSMComponent::ChangeState // No CharacterMonster"));
						check(false);
					}
					break;
				case EHinoxCombatIndex::HandClap:
					if (CharacterMonster)
					{
						CharacterMonster->PlayMontage(EMonsterMontage::HANDCLAP);
					}
					else
					{
						UE_LOG(LogTemp, Error, TEXT("ULynelFSMComponent::ChangeState // No CharacterMonster"));
						check(false);
					}
					break;
				case EHinoxCombatIndex::LeftHand:
					if (CharacterMonster)
					{
						CharacterMonster->PlayMontage(EMonsterMontage::LEFTHAND);
					}
					else
					{
						UE_LOG(LogTemp, Error, TEXT("ULynelFSMComponent::ChangeState // No CharacterMonster"));
						check(false);
					}
					break;
				case EHinoxCombatIndex::Hipdrop:
					if (CharacterMonster)
					{
						CharacterMonster->PlayMontage(EMonsterMontage::HIPDROP);
					}
					else
					{
						UE_LOG(LogTemp, Error, TEXT("ULynelFSMComponent::ChangeState // No CharacterMonster"));
						check(false);
					}
					break;
				case EHinoxCombatIndex::ThrowStone:
					if (CharacterMonster)
					{
						CharacterMonster->PlayMontage(EMonsterMontage::THROW_STONE_START);
					}
					else
					{
						UE_LOG(LogTemp, Error, TEXT("ULynelFSMComponent::ChangeState // No CharacterMonster"));
						check(false);
					}
					break;
				case EHinoxCombatIndex::End:
					break;
				default:
					break;
				}
			}
		}
	}

	// Player is Far
	else
	{
		FVector ToPlayer = PlayerLocation - CharacterMonster->GetActorLocation();
		ToPlayer.Z = 0.0f; // 수평 방향만 비교
		ToPlayer.Normalize();

		FVector Forward = CharacterMonster->GetActorForwardVector();
		Forward.Z = 0.0f;
		Forward.Normalize();

		// 오른쪽 기준 벡터 (몬스터의 오른쪽)
		FVector Right = FVector::CrossProduct(FVector::UpVector, Forward);

		float ForwardDot = FVector::DotProduct(Forward, ToPlayer);
		float RightDot = FVector::DotProduct(Right, ToPlayer);


		SmoothRotateActorToDirection(CharacterMonster, PlayerLocation, DeltaTime, 1.f);


		// 정면 조건: ForwardDot이 높고, 좌우 편차가 작을 때
		if (ForwardDot > 0.7f && FMath::Abs(RightDot) < 0.3f
			&& !CharacterMonster->IsPlayingMontage(EMonsterMontage::RUN)) // 현재 몬스터가 RUN 몽타주를 재생 중이 아닐 때)
		{
			CharacterMonster->PlayMontage(EMonsterMontage::RUN); // 정면
		}
		else if (RightDot > 0.3f)
		{
			CharacterMonster->PlayMontage(EMonsterMontage::RUN_CURVE_R); // 오른쪽
		}
		else if (RightDot < -0.3f)
		{
			CharacterMonster->PlayMontage(EMonsterMontage::RUN_CURVE_L); // 왼쪽
		}
	}
}

void UHinoxFSMComponent::UpdateDying(float DeltaTime)
{
	this->StopMove();
	Super::UpdateDying(DeltaTime);
}

void UHinoxFSMComponent::UpdateDamageEye(float DeltaTime)
{
	if (!Player)
	{
		UE_LOG(LogTemp, Warning, TEXT("UHinoxFSMComponent::UpdateDamageEye // No Player"));
		ChangeState(EMonsterState::Idle);
		return;
	}
	DamageEyeElapsedTime += DeltaTime;
	if (DamageEyeElapsedTime > HINOX_DAMAGE_EYE_MAX_TIME)
	{
		DamageEyeElapsedTime = 0.f;
		ChangeState(EMonsterState::Combat);
		return;
	}
	this->StopMove();
}



void UHinoxFSMComponent::UpdateTemp(float DeltaTime)
{
	this->StopMove();
}

void UHinoxFSMComponent::UpdateCombatIndex()
{
	switch (eCombatIndex)
	{
	case EHinoxCombatIndex::Foot:
		eCombatIndex = EHinoxCombatIndex::HandClap;
		break;
	case EHinoxCombatIndex::HandClap:
		eCombatIndex = EHinoxCombatIndex::LeftHand;
		break;
	case EHinoxCombatIndex::LeftHand:
		eCombatIndex = EHinoxCombatIndex::Hipdrop;
		break;
	case EHinoxCombatIndex::Hipdrop:
		eCombatIndex = EHinoxCombatIndex::ThrowStone;
		break;
	case EHinoxCombatIndex::ThrowStone:
		eCombatIndex = EHinoxCombatIndex::Foot;
		break;
	default:
		break;
	}
}
