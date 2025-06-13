// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/FSMComponent/Monster/MonsterFSMComponent.h"
#include "Components/StatusComponent/MonsterStatusComponent/MonsterStatusComponent.h"

#include "Actors/Object/PatrolPath.h"
#include "Actors/Object/CampFire.h"
#include "Actors/Controller/AIController/Monster/MonsterAIController.h"
#include "Actors/Character/PlayerCharacter.h"
#include "Actors/Projectile/Projectile.h"
#include "Actors/Item/WorldWeapon.h"
#include "Actors/Monster/MonsterInterface.h"
#include "Actors/Monster/CharacterMonster.h"
#include "Actors/Monster/PawnMonster.h"


#include "Navigation/PathFollowingComponent.h"

#include "GameFramework/PC_InGame.h"

#include "UI/HUD/MainHUD.h"

#include "Data/MonsterTableRow.h"


UMonsterFSMComponent::UMonsterFSMComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UMonsterFSMComponent::BeginPlay()
{
	Super::BeginPlay();

	switch (eGroupType)
	{
	case EMonsterGroupType::Patrol:
		ChangeState(EMonsterState::Patrol);
		break;
	case EMonsterGroupType::TreasureBox:
		ChangeState(EMonsterState::Idle);
		break;
	case EMonsterGroupType::Alone:
		//UE_LOG(LogTemp, Error, TEXT("UMonsterFSMComponent::BeginPlay // No GroupType"));
		//check(false);
		break;
	case EMonsterGroupType::End:
		UE_LOG(LogTemp, Error, TEXT("UMonsterFSMComponent::BeginPlay // No GroupType"));
		check(false);
		break;
	default:
		break;
	}


}


// Called every frame
void UMonsterFSMComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	HandleState(DeltaTime);
}

void UMonsterFSMComponent::OnHitReceived(bool bIsDead)
{
	if (bIsDead)
	{
		ChangeState(EMonsterState::Dead);
	}
	else
	{
		ChangeState(EMonsterState::Damage);
	}
}

void UMonsterFSMComponent::BindHitEvent()
{
	if (CharacterMonster)
	{
		CharacterMonster->GetStatusComponent()->OnHPChanged.AddDynamic(this, &ThisClass::UpdateUIHPBar);
	}
	else if (PawnMonster)
	{
		PawnMonster->GetStatusComponent()->OnHPChanged.AddDynamic(this, &ThisClass::UpdateUIHPBar);
	}
}

void UMonsterFSMComponent::SheathMeleeWeapon()
{
	CurrentWeapon = nullptr;
	if (CharacterMonster)
	{
		MeleeWeapon->AttachToMonster(CharacterMonster, Monster_SocketName::Pod_Melee);
	}
	else if (PawnMonster)
	{
		MeleeWeapon->AttachToMonster(PawnMonster, Monster_SocketName::Pod_Melee);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UMonsterFSMComponent::SheathMeleeWeapon // No CharacterMonster or PawnMonster"));
		check(false);
	}
}

void UMonsterFSMComponent::SheathBowWeapon()
{
	CurrentWeapon = nullptr;
	if (CharacterMonster)
	{ 
		BowWeapon->AttachToMonster(CharacterMonster, Monster_SocketName::Pod_Bow);
	}
	else if (PawnMonster)
	{
		MeleeWeapon->AttachToMonster(PawnMonster, Monster_SocketName::Pod_Bow);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UMonsterFSMComponent::SheathBowWeapon // No CharacterMonster or PawnMonster"));
		check(false);
	}
}

void UMonsterFSMComponent::DrawMeleeWeapon()
{
	CurrentWeapon = MeleeWeapon;
	if (CharacterMonster)
	{
		MeleeWeapon->AttachToMonster(CharacterMonster, Monster_SocketName::Weapon_Right);
	}
	else if (PawnMonster)
	{
		MeleeWeapon->AttachToMonster(PawnMonster, Monster_SocketName::Weapon_Right);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UMonsterFSMComponent::DrawMeleeWeapon // No CharacterMonster or PawnMonster"))
		check(false);
	}
}

void UMonsterFSMComponent::DrawBowWeapon()
{
	CurrentWeapon = BowWeapon;

	if (CharacterMonster)
	{
		BowWeapon->AttachToMonster(CharacterMonster, Monster_SocketName::Weapon_Right);
	}
	else if (PawnMonster)
	{
		BowWeapon->AttachToMonster(PawnMonster, Monster_SocketName::Weapon_Right);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UMonsterFSMComponent::DrawBowWeapon // No CharacterMonster or PawnMonster"));
		check(false);
	}
}

void UMonsterFSMComponent::DropWeapons()
{
	if (MeleeWeapon)
	{
		MeleeWeapon->DetachFromMonster();
		MeleeWeapon = nullptr;
	}
	if (BowWeapon)
	{ 
		BowWeapon->DetachFromMonster();
		BowWeapon = nullptr;
	}
	if (CurrentWeapon)
	{
		CurrentWeapon->DetachFromMonster();
		CurrentWeapon = nullptr;
	}
}

void UMonsterFSMComponent::SetMonsterGroupType(EMonsterGroupType NewGroupType)
{
	eGroupType = NewGroupType;

	switch (eGroupType)
	{
	case EMonsterGroupType::Patrol:
		if (EMonsterState::Idle == eCurrentState)
		{
			ChangeState(EMonsterState::Patrol);
		}
		break;
	case EMonsterGroupType::TreasureBox:
		if (EMonsterState::Patrol == eCurrentState)
		{
			ChangeState(EMonsterState::Idle);
		}
		break;
	case EMonsterGroupType::Alone:
		break;
	case EMonsterGroupType::End:
		break;
	default:
		break;
	}
}

void UMonsterFSMComponent::HandleState(float DeltaTime)
{
	if (!CharacterMonster && !PawnMonster)
	{
		UE_LOG(LogTemp, Error, TEXT("UMonsterFSMComponent::HandleState // No MonsterInterface.GetInterface()"));
		check(false);
		return;
	}

	switch (eCurrentState)
	{
	case EMonsterState::Idle:
		UpdateIdle(DeltaTime);
		break;
	case EMonsterState::Patrol:
		UpdatePatrol(DeltaTime);
		break;
	case EMonsterState::Suspicious:
		UpdateSuspicious(DeltaTime);
		break;
	case EMonsterState::Alert:
		UpdateAlert(DeltaTime);
		break;
	case EMonsterState::FindWeapon:
		UpdateFindWeapon(DeltaTime);
		break;
	case EMonsterState::Combat:
		UpdateCombat(DeltaTime);
		break;
	case EMonsterState::AimingBow:
		UpdateAimingBow(DeltaTime);
		break;
	case EMonsterState::Dance:
		UpdateDance(DeltaTime);
		break;
	case EMonsterState::ToDance:
		UpdateToDance(DeltaTime);
		break;
	case EMonsterState::Signal:
		UpdateSignal(DeltaTime);
		break;
	case EMonsterState::Dead:
		UpdateDying(DeltaTime);
		break;
	case EMonsterState::Damage:
		UpdateDamage(DeltaTime);
		break;
	case EMonsterState::Stun:
		UpdateStun(DeltaTime);
		break;
	default:
		UE_LOG(LogTemp, Error, TEXT("UMonsterFSMComponent::HandleState // Unexpected MonsterState"));
		check(false);
		break;
	}
}

void UMonsterFSMComponent::ChangeState(EMonsterState NewState)
{
	if (eCurrentState == NewState) { return; }

	const EMonsterState PrevState = eCurrentState;
	switch (PrevState)
	{
	case EMonsterState::Idle:
		break;
	case EMonsterState::Patrol:
		break;
	case EMonsterState::Suspicious:
		SuspicionGauge = 0.f;
		break;
	case EMonsterState::Alert:
		break;
	case EMonsterState::Combat:
		if (PawnMonster)
		{
			PawnMonster->SetSpeedRun();
		}
		else if (CharacterMonster)
		{
			CharacterMonster->SetSpeedRun();
		}
		break;
	case EMonsterState::FindWeapon:
		break;

	case EMonsterState::Dead:
		break;
	case EMonsterState::Fire:
		break;
	case EMonsterState::Eat:
		break;
	case EMonsterState::Dance:
		break;
	case EMonsterState::Signal:
		if (PawnMonster)
		{
			PawnMonster->PlayMontage(EMonsterMontage::SIGNAL_END);
		}
		else if (CharacterMonster)
		{
			CharacterMonster->PlayMontage(EMonsterMontage::SIGNAL_END);
		}
		break;
	case EMonsterState::AimingBow:
		break;
	case EMonsterState::Damage:
		break;
	case EMonsterState::End:
		break;
	default:
		break;
	}


	switch (NewState)
	{
	case EMonsterState::Idle:
		break;
	case EMonsterState::Patrol:
		break;
	case EMonsterState::Suspicious:
		break;
	case EMonsterState::Alert:
		if (PawnMonster)
		{
			PawnMonster->PlayMontage(EMonsterMontage::FIND);
		}
		else if (CharacterMonster)
		{
			CharacterMonster->PlayMontage(EMonsterMontage::FIND);
		}
		break;
	case EMonsterState::FindWeapon:
		if (CurrentWeapon)
		{
			return;
		}

		if (PrevState != EMonsterState::Combat)
		{
			if (PawnMonster)
			{
				PawnMonster->PlayMontage(EMonsterMontage::ANGRY);
				PawnMonster->SetSpeedRun();
			}
			else if (CharacterMonster)
			{
				CharacterMonster->PlayMontage(EMonsterMontage::ANGRY);
				CharacterMonster->SetSpeedRun();
			}
		}
		break;
	case EMonsterState::Combat:
		break;
	case EMonsterState::AimingBow:
		break;
	case EMonsterState::Dance:
		if (PawnMonster)
		{
			PawnMonster->PlayMontage(EMonsterMontage::DANCE_START);
		}
		else if (CharacterMonster)
		{
			CharacterMonster->PlayMontage(EMonsterMontage::DANCE_START);
		}
		break;
	case EMonsterState::Signal:
	{
		if (PawnMonster)
		{
			PawnMonster->PlayMontage(EMonsterMontage::SIGNAL_START);
			//SpawnProjectile(ProjectileName::Monster_PlayerAlert, CollisionProfileName::ToMonster);
		}
		else if (CharacterMonster)
		{
			CharacterMonster->PlayMontage(EMonsterMontage::SIGNAL_START);
		}
	}
		break;
	case EMonsterState::Dead:
		if (PawnMonster)
		{
			PawnMonster->PlayMontage(EMonsterMontage::DEAD);
		}
		else if (CharacterMonster)
		{
			CharacterMonster->PlayMontage(EMonsterMontage::DEAD);
		}
		break;
	case EMonsterState::Damage:
		if (PawnMonster)
		{
			PawnMonster->PlayMontage(EMonsterMontage::DAMAGE);
		}
		else if (CharacterMonster)
		{
			CharacterMonster->PlayMontage(EMonsterMontage::DAMAGE);
		}
		break;
	case EMonsterState::Stun:
		if (PawnMonster)
		{
			PawnMonster->PlayMontage(EMonsterMontage::STUN_START);
		}
		else if (CharacterMonster)
		{
			CharacterMonster->PlayMontage(EMonsterMontage::STUN_START);
		}
		break;
	default:
		break;
	}
	eCurrentState = NewState;
}

void UMonsterFSMComponent::UpdateIdle(float DeltaTime)
{
	// if it is PatrolGroup, it's error
	if (eGroupType == EMonsterGroupType::Patrol)
	{
		UE_LOG(LogTemp, Error, TEXT("UMonsterFSMComponent::UpdatePatrol // This Monster is PatrolGroup, not TreasureBoxGroup"));
		ChangeState(EMonsterState::Patrol);
		return;
	}


	ChangeState(EMonsterState::ToDance);
}

void UMonsterFSMComponent::UpdateDance(float DeltaTime)
{
	if (!CharacterMonster && !PawnMonster)
	{
		UE_LOG(LogTemp, Error, TEXT("UMonsterFSMComponent::UpdateDance // No CharacterMonster or PawnMonster"));
		check(false);
		return;
	}

	
	this->StopMove();

	if (PawnMonster)
	{
		if (AActor* CampFireActor = PawnMonster->GetCampFire())
		{
			const FVector CampFireLocation = CampFireActor->GetActorLocation();
			SmoothRotateActorToDirection(PawnMonster, CampFireLocation, DeltaTime);
		}
	}
	else if (CharacterMonster)
	{
		if (AActor* CampFireActor = CharacterMonster->GetCampFire())
		{
			const FVector CampFireLocation = CampFireActor->GetActorLocation();
			SmoothRotateActorToDirection(CharacterMonster, CampFireLocation, DeltaTime);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UMonsterFSMComponent::UpdateDance // No CharacterMonster or PawnMonster"));
		check(false);
		return;
	}

	if (Player)
	{
		ChangeState(EMonsterState::Suspicious);
	}
}

void UMonsterFSMComponent::UpdateToDance(float DeltaTime)
{
	if (!CharacterMonster && !PawnMonster)
	{
		UE_LOG(LogTemp, Error, TEXT("UMonsterFSMComponent::UpdateDance // No CharacterMonster or PawnMonster"));
		check(false);
		return;
	}

	if (PawnMonster)
	{
		if (AActor* CampFireActor = PawnMonster->GetCampFire())
		{
			const FVector CampFireLocation = CampFireActor->GetActorLocation();
			MoveToLocation(CampFireLocation);
			float fDistance = FVector::Dist(PawnMonster->GetActorLocation(), CampFireLocation);
			if (fDistance < MONSTER_CAMPFIRE_MIN_LENGTH)
			{
				this->StopMove();
				ChangeState(EMonsterState::Dance);
			}
		}
	}
	else if (CharacterMonster)
	{
		if (AActor* CampFireActor = CharacterMonster->GetCampFire())
		{
			const FVector CampFireLocation = CampFireActor->GetActorLocation();
			MoveToLocation(CampFireLocation);
			float fDistance = FVector::Dist(CharacterMonster->GetActorLocation(), CampFireLocation);
			if (fDistance < MONSTER_CAMPFIRE_MIN_LENGTH)
			{
				this->StopMove();
				ChangeState(EMonsterState::Dance);
			}
		}
	}

	if (Player)
	{
		ChangeState(EMonsterState::Suspicious);
	}
}

void UMonsterFSMComponent::UpdatePatrol(float DeltaTime)
{
	// if it is TreasureBoxGroup, it's error
	if (eGroupType == EMonsterGroupType::TreasureBox)
	{
		UE_LOG(LogTemp, Error, TEXT("UMonsterFSMComponent::UpdatePatrol // This Monster is TreasureBoxGroup, not PatrolGroup"));
		ChangeState(EMonsterState::Patrol);
		return;
	}


	// 목표 위치 구하기
	FVector Location = FVector();

	if (PawnMonster)
	{
		if (PatrolPath)
		{
			Location = PatrolPath->GetSplinePointLocation(CurrentPatrolIndex);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("UMonsterFSMComponent::UpdatePatrol // No PatrolPath"));
			check(false);
			return;
		}
	}
	else if (CharacterMonster)
	{
		if (PatrolPath)
		{
			Location = PatrolPath->GetSplinePointLocation(CurrentPatrolIndex);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("UMonsterFSMComponent::UpdatePatrol // No PatrolPath"));
			check(false);
			return;
		}
	}

	MoveToLocation(Location);

	// 다음 PatrolIndex 구하기
	AActor* TempActor = CharacterMonster ? Cast<AActor>(CharacterMonster) : Cast<AActor>(PawnMonster);

	const bool bIsNear = FVector::PointsAreNear(TempActor->GetActorLocation(), Location, MONSTER_DEFAULT_NEAR_DISTANCE);

	if (bIsNear)
	{
		++CurrentPatrolIndex;

		if (PatrolPath)
		{
			if (CurrentPatrolIndex >= PatrolPath->GetSplineMaxIndex())
			{
				CurrentPatrolIndex = 0;
			}
		}
	}

	if (Player)
	{
		this->StopMove();
		ChangeState(EMonsterState::Suspicious);
	}
}

void UMonsterFSMComponent::UpdateSuspicious(float DeltaTime)
{
	if (Player)
	{
		this->StopMove();
		SuspicionGauge += DeltaTime * MONSTER_SUSPICIOUS_COEFFICIENT;

		AActor* TempActor = CharacterMonster ? Cast<AActor>(CharacterMonster) : Cast<AActor>(PawnMonster);

		const FVector MonsterLocation = TempActor->GetActorLocation();
		const FVector PlayerLocation = Player->GetActorLocation();
		const float fDistance = FVector::Dist(MonsterLocation, PlayerLocation);

		if (SuspicionGauge >= MONSTER_MAX_SUSPICIOUS_GAUGE
			|| fDistance < MONSTER_IMMEDIATE_ALERT_RADIUS
			)
		{
			SuspicionGauge = 0.f;
			ChangeState(EMonsterState::Alert);
			return;
		}
		else if (fDistance > MONSTER_AISENSECONFIG_SIGHT_LOSESIGHTRADIUS)
		{
			ChangeState(EMonsterState::Idle);
			Player = nullptr;
			return;
		}
		else
		{
			SuspicionGauge += DeltaTime;
		}

		SmoothRotateActorToDirection(TempActor, PlayerLocation, DeltaTime);

	}
	else
	{
		ChangeState(EMonsterState::Idle);
		return;
	}
}

void UMonsterFSMComponent::UpdateAlert(float DeltaTime)
{
	// Actually do nothing in here
}

void UMonsterFSMComponent::UpdateFindWeapon(float DeltaTime)
{
	if (!IsValid(Player))
	{
		ToCatchWeapon = nullptr;
		ChangeState(EMonsterState::Idle);
		return;
	}

	if (ToCatchWeapon)
	{
		// Already Owned by other Monster or Player
		if (ToCatchWeapon->GetCatched())
		{
			ChangeState(EMonsterState::Combat);
			return;
		}

		if (CurrentWeapon)
		{
			UE_LOG(LogTemp, Error, TEXT("UMonsterFSMComponent::UpdateFindWeapon // ToCatchWeapon, CatchedWeapon can't exist together "));
			check(false);
			return;
		}

		// 이동
		// Get Target Location
		AActor* TempActor = CharacterMonster ? Cast<AActor>(CharacterMonster) : Cast<AActor>(PawnMonster);

		FVector WeaponLocation = ToCatchWeapon->GetActorLocation();
		FVector MonsterLocation = TempActor->GetActorLocation();
		FVector PlayerLocation = Player->GetActorLocation();

		const float fDistance = FVector::Dist(PlayerLocation, MonsterLocation);
		if (fDistance > MONSTER_AISENSECONFIG_SIGHT_LOSESIGHTRADIUS)
		{
			ToCatchWeapon = nullptr;
			Player = nullptr;
			ChangeState(EMonsterState::Idle);
			return;
		}


		if (PawnMonster && PawnMonster->IsPlayingMontage(EMonsterMontage::END))
		{
			this->StopMove();
		}
		else
		{
			MoveToLocation(WeaponLocation);
		}

		const bool bIsNear = FVector::PointsAreNear(MonsterLocation, WeaponLocation, MONSTER_DEFAULT_NEAR_DISTANCE);

		if (bIsNear)
		{
			this->StopMove();
			InstantRotateActorToDirection(TempActor, WeaponLocation);
			if (PawnMonster)
			{
				PawnMonster->PlayMontage(EMonsterMontage::WEAPON_CATCH);
			}
		}
	}
	else
	{
		ChangeState(EMonsterState::Combat);
	}
}

void UMonsterFSMComponent::UpdateCombat(float DeltaTime)
{
	if (!IsValid(Player))
	{
		ChangeState(EMonsterState::Idle);
		return;
	}

	// Get Target Location
	FVector Location = Player->GetActorLocation();
	AActor* TempActor = CharacterMonster ? Cast<AActor>(CharacterMonster) : Cast<AActor>(PawnMonster);

	FVector MonsterLocation = TempActor->GetActorLocation();

	const float fDistance = FVector::Dist(Location, MonsterLocation);
	if (fDistance > MONSTER_AISENSECONFIG_SIGHT_LOSESIGHTRADIUS)
	{
		Player = nullptr;
		ChangeState(EMonsterState::Idle);
		return;
	}


	if (PawnMonster && PawnMonster->IsPlayingMontage(EMonsterMontage::END))
	{
		this->StopMove();
	}
	else
	{
		MoveToLocation(Location);
	}

	// 공격 쿨타임 갱신
	CurrentAttackCoolTime += DeltaTime;



	// Check if it's arrived
	const bool bIsNear = FVector::PointsAreNear(MonsterLocation, Location, MONSTER_DEFAULT_NEAR_DISTANCE);

	if (bIsNear) this->StopMove();

	if (CurrentAttackCoolTime > MONSTER_ATTACK_COOLTIME)
	{
		if (CurrentWeapon)
		{
			const EWeaponKind eWeaponKind = CurrentWeapon->GetWorldWeaponKind();
			switch (eWeaponKind)
			{
			case EWeaponKind::SWORD:
				if (bIsNear)
				{
					CurrentAttackCoolTime = 0.f;
					if (PawnMonster)
					{
						PawnMonster->PlayMontage(EMonsterMontage::ATTACK_SWORD);
					}
					else if (CharacterMonster)
					{
						CharacterMonster->PlayMontage(EMonsterMontage::ATTACK_SWORD);
					}
				}
				break;
			case EWeaponKind::SPEAR:
				if (bIsNear)
				{
					CurrentAttackCoolTime = 0.f;
					if (PawnMonster)
					{
						PawnMonster->PlayMontage(EMonsterMontage::ATTACK_SPEAR);
					}
					else if (CharacterMonster)
					{
						CharacterMonster->PlayMontage(EMonsterMontage::ATTACK_SPEAR);
					}
				}
				break;
			case EWeaponKind::LSWORD:
				if (bIsNear)
				{
					CurrentAttackCoolTime = 0.f;
					if (PawnMonster)
					{
						PawnMonster->PlayMontage(EMonsterMontage::ATTACK_LSWORD);
					}
					else if (CharacterMonster)
					{
						CharacterMonster->PlayMontage(EMonsterMontage::ATTACK_LSWORD);
					}
				}
				break;
			case EWeaponKind::BOW:
				CurrentAttackCoolTime = 0.f;
				if (PawnMonster)
				{
					PawnMonster->PlayMontage(EMonsterMontage::BOW_START);
				}
				else if (CharacterMonster)
				{
					CharacterMonster->PlayMontage(EMonsterMontage::BOW_START);
				}
				ChangeState(EMonsterState::AimingBow);
				break;
			case EWeaponKind::END:
			default:
				UE_LOG(LogTemp, Error, TEXT("UMonsterFSMComponent::UpdateCombat // No Weapon Kind"))
					check(false);
				break;
			}
		}
		else
		{
			if (CurrentAttackCoolTime > MONSTER_ATTACK_COOLTIME)
			{
				if (bIsNear)
				{
					CurrentAttackCoolTime = 0.f;
					if (PawnMonster)
					{
						PawnMonster->PlayMontage(EMonsterMontage::ATTACK);
					}
					else if (CharacterMonster)
					{
						CharacterMonster->PlayMontage(EMonsterMontage::ATTACK);
					}
				}
			}
		}
	}
}

void UMonsterFSMComponent::UpdateSignal(float DeltaTime)
{
	SignalElapsedTime += DeltaTime;
	this->StopMove();
	AActor* TempActor = CharacterMonster ? Cast<AActor>(CharacterMonster) : Cast<AActor>(PawnMonster);

	const FVector PlayerLocation = Player->GetActorLocation();

	SmoothRotateActorToDirection(TempActor, PlayerLocation, DeltaTime);
	if (SignalElapsedTime > MONSTER_MAX_SIGNAL_TIME)
	{
		ChangeState(EMonsterState::Combat);
	}
}

void UMonsterFSMComponent::UpdateAimingBow(float DeltaTime)
{
	if (!Player)
	{
		AimingBowElapsedTime = 0.f;
		ChangeState(EMonsterState::Idle);
		return;
	}

	this->StopMove();
	AimingBowElapsedTime += DeltaTime;
	const FVector PlayerLocation = Player->GetActorLocation();
	AActor* TempActor = CharacterMonster ? Cast<AActor>(CharacterMonster) : Cast<AActor>(PawnMonster);

	SmoothRotateActorToDirection(TempActor, PlayerLocation, DeltaTime);

	if (AimingBowElapsedTime > MONSTER_AIMINGBOW_MAX_TIME)
	{
		AimingBowElapsedTime = 0.f;
		if (PawnMonster)
		{
			PawnMonster->PlayMontage(EMonsterMontage::BOW_END);
		}
		else if (CharacterMonster)
		{
			CharacterMonster->PlayMontage(EMonsterMontage::BOW_END);
		}
	}
}

void UMonsterFSMComponent::UpdateDying(float DeltaTime)
{
	this->StopMove();

	if (CharacterMonster && !CharacterMonster->IsPlayingMontage(EMonsterMontage::DEAD))
	{
		DropWeapons();

		CharacterMonster->OnDeadEnd();
		CharacterMonster->Destroy();
	}
	else if (PawnMonster && !PawnMonster->IsPlayingMontage(EMonsterMontage::DEAD))
	{
		DropWeapons();
		PawnMonster->OnDeadEnd();
		PawnMonster->Destroy();
	}
}

void UMonsterFSMComponent::UpdateDamage(float DeltaTime)
{
	this->StopMove();

	if (PawnMonster && !PawnMonster->IsPlayingMontage(EMonsterMontage::DAMAGE))
	{
		ChangeState(EMonsterState::Combat);
	}
	else if (CharacterMonster && !CharacterMonster->IsPlayingMontage(EMonsterMontage::DAMAGE))
	{
		ChangeState(EMonsterState::Combat);
	}
}

void UMonsterFSMComponent::UpdateStun(float DeltaTime)
{
	this->StopMove();

	if (PawnMonster && !PawnMonster->IsPlayingMontage(EMonsterMontage::STUN_START))
	{
		ChangeState(EMonsterState::Combat);
	}
	else if (CharacterMonster && !CharacterMonster->IsPlayingMontage(EMonsterMontage::STUN_START))
	{
		ChangeState(EMonsterState::Combat);
	}
}

void UMonsterFSMComponent::MoveToLocation(const FVector& InLocation)
{
	if (CharacterMonster)
	{
		if (AAIController* AIController = Cast<AAIController>(CharacterMonster->GetController()))
		{
			FAIMoveRequest MoveRequest;
			MoveRequest.SetGoalLocation(InLocation);
			MoveRequest.SetAcceptanceRadius(50.f);

			FNavPathSharedPtr NavPath;
			AIController->MoveTo(MoveRequest, &NavPath);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("UMonsterFSMComponent::MoveToLocation // No AIController"));
			check(false);
		}
	}
	else if (PawnMonster)
	{
		if (AAIController* AIController = Cast<AAIController>(PawnMonster->GetController()))
		{
			FAIMoveRequest MoveRequest;
			MoveRequest.SetGoalLocation(InLocation);
			MoveRequest.SetAcceptanceRadius(50.f);

			FNavPathSharedPtr NavPath;
			AIController->MoveTo(MoveRequest, &NavPath);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("UMonsterFSMComponent::MoveToLocation // No AIController"));
			check(false);
		}
	}
}

void UMonsterFSMComponent::StopMove()
{
	AActor* TempActor = CharacterMonster ? Cast<AActor>(CharacterMonster) : Cast<AActor>(PawnMonster);

	if (CharacterMonster)
	{
		if (AAIController* AIController = Cast<AAIController>(CharacterMonster->GetController()))
		{
			AIController->StopMovement();
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("UMonsterFSMComponent::StopMove // No AIController"));
			check(false);
		}
	}
	else if (PawnMonster)
	{
		if (AAIController* AIController = Cast<AAIController>(PawnMonster->GetController()))
		{
			AIController->StopMovement();
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("UMonsterFSMComponent::StopMove // No AIController"));
			check(false);
		}
	}

}

void UMonsterFSMComponent::SpawnProjectile(FName ProjectileName, FName CollisionProfileName)
{
	UWorld* World = GetWorld();

	AActor* TempActor = CharacterMonster ? Cast<AActor>(CharacterMonster) : Cast<AActor>(PawnMonster);

	AProjectile* Projectile = World->SpawnActorDeferred<AProjectile>(AProjectile::StaticClass(),
		FTransform::Identity, TempActor, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

	FTransform NewTransform;
	Projectile->SetData(ProjectileName, CollisionProfileName);
	const FVector Location = TempActor->GetActorLocation();

	NewTransform.SetLocation(Location);
	NewTransform.SetRotation(FRotator::ZeroRotator.Quaternion());
	Projectile->FinishSpawning(NewTransform);
}

void UMonsterFSMComponent::UpdateUIHPBar(float CurrentHP, float MaxHP)
{
	//if (CharacterMonster)
	//{ 
	//	if (UWorld* World = CharacterMonster->GetWorld())
	//	{
	//		if (APC_InGame* PC = Cast<APC_InGame>(World->GetFirstPlayerController()))
	//		{
	//			if (AMainHUD* HUD = Cast<AMainHUD>(PC->GetHUD()))
	//			{
	//				HUD->ShowBossHpUI(true, CurrentHP, MaxHP, CharacterMonster->GetMonsterData()->Name.ToString());
	//			}
	//		}
	//	}
	//}
	//else if (PawnMonster)
	//{
	//	if (UWorld* World = PawnMonster->GetWorld())
	//	{
	//		if (APC_InGame* PC = Cast<APC_InGame>(World->GetFirstPlayerController()))
	//		{
	//			if (AMainHUD* HUD = Cast<AMainHUD>(PC->GetHUD()))
	//			{
	//				HUD->ShowBossHpUI(false, CurrentHP, MaxHP, PawnMonster->GetMonsterData()->Name.ToString());
	//			}
	//		}
	//	}
	//}
}
