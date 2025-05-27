// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Monster/MonsterInterface.h"
#include "Actors/Item/WorldWeapon.h"
#include "Actors/Controller/AIController/Monster/MonsterAIController.h"
#include "Actors/Projectile/Projectile.h"

#include "Components/FSMComponent/Monster/MonsterFSMComponent.h"
#include "Components/StatusComponent/MonsterStatusComponent/MonsterStatusComponent.h"

#include "Data/MonsterTableRow.h"
#include "Data/ItemDataRow.h"

#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

#include "Data/MonsterTableRow.h"

void IMonsterInterface::PlayMontage(EMonsterMontage _InEnum, bool bIsLoop)
{
	UAnimInstance* AnimInstance = GetAnimInstance();
	FMonsterTableRow* MonsterData = GetMonsterData();
	if (!MonsterData) return;

	UAnimMontage* TempAnimMontage = nullptr;
	switch (_InEnum)
	{
	case EMonsterMontage::ATTACK:
		TempAnimMontage = MonsterData->AttackMontage;
		break;
	case EMonsterMontage::ATTACK_BIG:
		TempAnimMontage = MonsterData->AttackBigMontage;
		break;
	case EMonsterMontage::ATTACK_SPEAR:
		TempAnimMontage = MonsterData->AttackSpearMontage;
		break;
	case EMonsterMontage::ATTACK_SWORD:
		TempAnimMontage = MonsterData->AttackSwordMontage;
		break;
	case EMonsterMontage::ATTACK_LSWORD:
		TempAnimMontage = MonsterData->AttackLSwordMontage;
		break;
	case EMonsterMontage::ATTACK_THROW:
		TempAnimMontage = MonsterData->AttackThrowMontage;
		break;
	case EMonsterMontage::JUMP_START:
		TempAnimMontage = MonsterData->JumpStartMontage;
		break;
	case EMonsterMontage::JUMP_END:
		TempAnimMontage = MonsterData->JumpEndMontage;
		break;
	case EMonsterMontage::DAMAGE:
		TempAnimMontage = MonsterData->DamageMontage;
		break;
	case EMonsterMontage::ANGRY:
		TempAnimMontage = MonsterData->AngryMontage;
		break;
	case EMonsterMontage::BOW_START:
		TempAnimMontage = MonsterData->BowStartMontage;
		break;
	case EMonsterMontage::BOW_END:
		TempAnimMontage = MonsterData->BowEndMontage;
		break;
	case EMonsterMontage::THROW:
		TempAnimMontage = MonsterData->ThrowMontage;
		break;
	case EMonsterMontage::DANCE_START:
		TempAnimMontage = MonsterData->DanceStartMontage;
		break;
	case EMonsterMontage::DANCE_END:
		TempAnimMontage = MonsterData->DanceEndMontage;
		break;
	case EMonsterMontage::WEAPON_CATCH:
		TempAnimMontage = MonsterData->WeaponCatchMontage;
		break;
	case EMonsterMontage::FIND:
		TempAnimMontage = MonsterData->FindMontage;
		break;
	case EMonsterMontage::SIGNAL_START:
		TempAnimMontage = MonsterData->SignalStartMontage;
		break;
	case EMonsterMontage::SIGNAL_END:
		TempAnimMontage = MonsterData->SignalEndMontage;
		break;
	case EMonsterMontage::APPEAR:
		TempAnimMontage = MonsterData->APPEAR;
		break;
	case EMonsterMontage::BOW_UPPER_START:
		TempAnimMontage = MonsterData->BOW_UPPER_START;
		break;
	case EMonsterMontage::BOW_UPPER_END:
		TempAnimMontage = MonsterData->BOW_UPPER_END;
		break;
	case EMonsterMontage::ATTACK_DASH_LSWORD_START:
		TempAnimMontage = MonsterData->ATTACK_DASH_LSWORD_START;
		break;
	case EMonsterMontage::ATTACK_DASH_LSWORD_END:
		TempAnimMontage = MonsterData->ATTACK_DASH_LSWORD_END;
		break;
	case EMonsterMontage::ATTACK_DASH_SWORD_START:
		TempAnimMontage = MonsterData->ATTACK_DASH_SWORD_START;
		break;
	case EMonsterMontage::ATTACK_DASH_SWORD_END:
		TempAnimMontage = MonsterData->ATTACK_DASH_SWORD_END;
		break;
	case EMonsterMontage::ATTACK_EXPLOSION_START:
		TempAnimMontage = MonsterData->ATTACK_EXPLOSION_START;
		break;
	case EMonsterMontage::ATTACK_EXPLOSION_END:
		TempAnimMontage = MonsterData->ATTACK_EXPLOSION_END;
		break;
	case EMonsterMontage::ATTACK_FIRE_START:
		TempAnimMontage = MonsterData->ATTACK_FIRE_START;
		break;
	case EMonsterMontage::ATTACK_FIRE:
		TempAnimMontage = MonsterData->ATTACK_FIRE;
		break;
	case EMonsterMontage::ATTACK_FIRE_END:
		TempAnimMontage = MonsterData->ATTACK_FIRE_END;
		break;
	case EMonsterMontage::ATTACK_HORN_START:
		TempAnimMontage = MonsterData->ATTACK_HORN_START;
		break;
	case EMonsterMontage::ATTACK_HORN_END:
		TempAnimMontage = MonsterData->ATTACK_HORN_END;
		break;
	case EMonsterMontage::ATTACK_RUNNING_LSWORD_START:
		TempAnimMontage = MonsterData->ATTACK_RUNNING_LSWORD_START;
		break;
	case EMonsterMontage::ATTACK_RUNNING_LSWORD_END:
		TempAnimMontage = MonsterData->ATTACK_RUNNING_LSWORD_END;
		break;
	case EMonsterMontage::ATTACK_RUNNING_SWORD_START:
		TempAnimMontage = MonsterData->ATTACK_RUNNING_SWORD_START;
		break;
	case EMonsterMontage::ATTACK_RUNNING_SWORD_END:
		TempAnimMontage = MonsterData->ATTACK_RUNNING_SWORD_END;
		break;
	case EMonsterMontage::REBOUND:
		TempAnimMontage = MonsterData->REBOUND;
		break;
	case EMonsterMontage::RODEO_START:
		TempAnimMontage = MonsterData->RODEO_START;
		break;
	case EMonsterMontage::RODEO_END:
		TempAnimMontage = MonsterData->RODEO_END;
		break;
	case EMonsterMontage::STUN_START:
		TempAnimMontage = MonsterData->STUN_START;
		break;
	case EMonsterMontage::STUN_END:
		TempAnimMontage = MonsterData->STUN_END;
		break;
	case EMonsterMontage::TURN_180_L:
		TempAnimMontage = MonsterData->TURN_180_L;
		break;
	case EMonsterMontage::TURN_180_R:
		TempAnimMontage = MonsterData->TURN_180_R;
		break;
	case EMonsterMontage::DRAW_BOW:
		TempAnimMontage = MonsterData->DRAW_BOW;
		break;
	case EMonsterMontage::SHEATH_BOW:
		TempAnimMontage = MonsterData->SHEATH_BOW;
		break;
	case EMonsterMontage::BOW_TO_SWORD:
		TempAnimMontage = MonsterData->BOW_TO_SWORD;
		break;
	case EMonsterMontage::SWORD_TO_BOW:
		TempAnimMontage = MonsterData->SWORD_TO_BOW;
		break;
	case EMonsterMontage::DRAW_LSWORD:
		TempAnimMontage = MonsterData->DRAW_LSWORD;
		break;
	case EMonsterMontage::SHEATH_LSWORD:
		TempAnimMontage = MonsterData->SHEATH_LSWORD;
		break;
	case EMonsterMontage::GEAR_1_FORWARD:
		TempAnimMontage = MonsterData->Gear_1_Forward;
		break;
	case EMonsterMontage::GEAR_1_LEFT:
		TempAnimMontage = MonsterData->Gear_1_Left;
		break;
	case EMonsterMontage::GEAR_1_RIGHT:
		TempAnimMontage = MonsterData->Gear_1_Right;
		break;
	case EMonsterMontage::GEAR_2_FORWARD:
		TempAnimMontage = MonsterData->Gear_2_Forward;
		break;
	case EMonsterMontage::GEAR_2_LEFT:
		TempAnimMontage = MonsterData->Gear_2_Left;
		break;
	case EMonsterMontage::GEAR_2_RIGHT:
		TempAnimMontage = MonsterData->Gear_2_Right;
		break;
	case EMonsterMontage::GEAR_3_FORWARD:
		TempAnimMontage = MonsterData->Gear_3_Forward;
		break;
	case EMonsterMontage::GEAR_3_LEFT:
		TempAnimMontage = MonsterData->Gear_3_Left;
		UE_LOG(LogTemp, Warning, TEXT("EMonsterMontage::GEAR_3_LEFT"));
		break;
	case EMonsterMontage::GEAR_3_RIGHT:
		TempAnimMontage = MonsterData->Gear_3_Right;
		UE_LOG(LogTemp, Warning, TEXT("EMonsterMontage::GEAR_3_RIGHT"));
		break;
	case EMonsterMontage::END:
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
	else
	{
		int a = 0;
	}
}

bool IMonsterInterface::IsMontage(EMonsterMontage _InEnum)
{
	FMonsterTableRow* MonsterData = GetMonsterData();

	if (!MonsterData) return false;

	UAnimMontage* TempAnimMontage = nullptr;
	switch (_InEnum)
	{
	case EMonsterMontage::ATTACK:
		TempAnimMontage = MonsterData->AttackMontage;
		break;
	case EMonsterMontage::ATTACK_BIG:
		TempAnimMontage = MonsterData->AttackBigMontage;
		break;
	case EMonsterMontage::ATTACK_SPEAR:
		TempAnimMontage = MonsterData->AttackSpearMontage;
		break;
	case EMonsterMontage::ATTACK_SWORD:
		TempAnimMontage = MonsterData->AttackSwordMontage;
		break;
	case EMonsterMontage::ATTACK_LSWORD:
		TempAnimMontage = MonsterData->AttackLSwordMontage;
		break;
	case EMonsterMontage::ATTACK_THROW:
		TempAnimMontage = MonsterData->AttackThrowMontage;
		break;
	case EMonsterMontage::JUMP_START:
		TempAnimMontage = MonsterData->JumpStartMontage;
		break;
	case EMonsterMontage::JUMP_END:
		TempAnimMontage = MonsterData->JumpEndMontage;
		break;
	case EMonsterMontage::DAMAGE:
		TempAnimMontage = MonsterData->DamageMontage;
		break;
	case EMonsterMontage::ANGRY:
		TempAnimMontage = MonsterData->AngryMontage;
		break;
	case EMonsterMontage::BOW_START:
		TempAnimMontage = MonsterData->BowStartMontage;
		break;
	case EMonsterMontage::BOW_END:
		TempAnimMontage = MonsterData->BowEndMontage;
		break;
	case EMonsterMontage::THROW:
		TempAnimMontage = MonsterData->ThrowMontage;
		break;
	case EMonsterMontage::DANCE_START:
		TempAnimMontage = MonsterData->DanceStartMontage;
		break;
	case EMonsterMontage::DANCE_END:
		TempAnimMontage = MonsterData->DanceEndMontage;
		break;
	case EMonsterMontage::WEAPON_CATCH:
		TempAnimMontage = MonsterData->WeaponCatchMontage;
		break;
	case EMonsterMontage::FIND:
		TempAnimMontage = MonsterData->FindMontage;
		break;
	case EMonsterMontage::SIGNAL_START:
		TempAnimMontage = MonsterData->SignalStartMontage;
		break;
	case EMonsterMontage::SIGNAL_END:
		TempAnimMontage = MonsterData->SignalEndMontage;
		break;
	case EMonsterMontage::APPEAR:
		TempAnimMontage = MonsterData->APPEAR;
		break;
	case EMonsterMontage::BOW_UPPER_START:
		TempAnimMontage = MonsterData->BOW_UPPER_START;
		break;
	case EMonsterMontage::BOW_UPPER_END:
		TempAnimMontage = MonsterData->BOW_UPPER_END;
		break;
	case EMonsterMontage::ATTACK_DASH_LSWORD_START:
		TempAnimMontage = MonsterData->ATTACK_DASH_LSWORD_START;
		break;
	case EMonsterMontage::ATTACK_DASH_LSWORD_END:
		TempAnimMontage = MonsterData->ATTACK_DASH_LSWORD_END;
		break;
	case EMonsterMontage::ATTACK_DASH_SWORD_START:
		TempAnimMontage = MonsterData->ATTACK_DASH_SWORD_START;
		break;
	case EMonsterMontage::ATTACK_DASH_SWORD_END:
		TempAnimMontage = MonsterData->ATTACK_DASH_SWORD_END;
		break;
	case EMonsterMontage::ATTACK_EXPLOSION_START:
		TempAnimMontage = MonsterData->ATTACK_EXPLOSION_START;
		break;
	case EMonsterMontage::ATTACK_EXPLOSION_END:
		TempAnimMontage = MonsterData->ATTACK_EXPLOSION_END;
		break;
	case EMonsterMontage::ATTACK_FIRE_START:
		TempAnimMontage = MonsterData->ATTACK_FIRE_START;
		break;
	case EMonsterMontage::ATTACK_FIRE:
		TempAnimMontage = MonsterData->ATTACK_FIRE;
		break;
	case EMonsterMontage::ATTACK_FIRE_END:
		TempAnimMontage = MonsterData->ATTACK_FIRE_END;
		break;
	case EMonsterMontage::ATTACK_HORN_START:
		TempAnimMontage = MonsterData->ATTACK_HORN_START;
		break;
	case EMonsterMontage::ATTACK_HORN_END:
		TempAnimMontage = MonsterData->ATTACK_HORN_END;
		break;
	case EMonsterMontage::ATTACK_RUNNING_LSWORD_START:
		TempAnimMontage = MonsterData->ATTACK_RUNNING_LSWORD_START;
		break;
	case EMonsterMontage::ATTACK_RUNNING_LSWORD_END:
		TempAnimMontage = MonsterData->ATTACK_RUNNING_LSWORD_END;
		break;
	case EMonsterMontage::ATTACK_RUNNING_SWORD_START:
		TempAnimMontage = MonsterData->ATTACK_RUNNING_SWORD_START;
		break;
	case EMonsterMontage::ATTACK_RUNNING_SWORD_END:
		TempAnimMontage = MonsterData->ATTACK_RUNNING_SWORD_END;
		break;
	case EMonsterMontage::REBOUND:
		TempAnimMontage = MonsterData->REBOUND;
		break;
	case EMonsterMontage::RODEO_START:
		TempAnimMontage = MonsterData->RODEO_START;
		break;
	case EMonsterMontage::RODEO_END:
		TempAnimMontage = MonsterData->RODEO_END;
		break;
	case EMonsterMontage::STUN_START:
		TempAnimMontage = MonsterData->STUN_START;
		break;
	case EMonsterMontage::STUN_END:
		TempAnimMontage = MonsterData->STUN_END;
		break;
	case EMonsterMontage::TURN_180_L:
		TempAnimMontage = MonsterData->TURN_180_L;
		break;
	case EMonsterMontage::TURN_180_R:
		TempAnimMontage = MonsterData->TURN_180_R;
		break;
	case EMonsterMontage::DRAW_BOW:
		TempAnimMontage = MonsterData->DRAW_BOW;
		break;
	case EMonsterMontage::SHEATH_BOW:
		TempAnimMontage = MonsterData->SHEATH_BOW;
		break;
	case EMonsterMontage::BOW_TO_SWORD:
		TempAnimMontage = MonsterData->BOW_TO_SWORD;
		break;
	case EMonsterMontage::SWORD_TO_BOW:
		TempAnimMontage = MonsterData->SWORD_TO_BOW;
		break;
	case EMonsterMontage::DRAW_LSWORD:
		TempAnimMontage = MonsterData->DRAW_LSWORD;
		break;
	case EMonsterMontage::SHEATH_LSWORD:
		TempAnimMontage = MonsterData->SHEATH_LSWORD;
		break;
	case EMonsterMontage::GEAR_1_FORWARD:
		TempAnimMontage = MonsterData->Gear_1_Forward;
		break;
	case EMonsterMontage::GEAR_1_LEFT:
		TempAnimMontage = MonsterData->Gear_1_Left;
		break;
	case EMonsterMontage::GEAR_1_RIGHT:
		TempAnimMontage = MonsterData->Gear_1_Right;
		break;
	case EMonsterMontage::GEAR_2_FORWARD:
		TempAnimMontage = MonsterData->Gear_2_Forward;
		break;
	case EMonsterMontage::GEAR_2_LEFT:
		TempAnimMontage = MonsterData->Gear_2_Left;
		break;
	case EMonsterMontage::GEAR_2_RIGHT:
		TempAnimMontage = MonsterData->Gear_2_Right;
		break;
	case EMonsterMontage::GEAR_3_FORWARD:
		TempAnimMontage = MonsterData->Gear_3_Forward;
		break;
	case EMonsterMontage::GEAR_3_LEFT:
		TempAnimMontage = MonsterData->Gear_3_Left;
		break;
	case EMonsterMontage::GEAR_3_RIGHT:
		TempAnimMontage = MonsterData->Gear_3_Right;
		break;
	case EMonsterMontage::END:
		TempAnimMontage = nullptr;
		break;
	default:
		break;
	}

	return TempAnimMontage ? true : false;
}

bool IMonsterInterface::IsPlayingMontage(EMonsterMontage _InEnum)
{
	FMonsterTableRow* MonsterData = GetMonsterData();

	if (!MonsterData) return false;
	UAnimInstance* AnimInstance = GetAnimInstance();

	UAnimMontage* TempAnimMontage = nullptr;
	switch (_InEnum)
	{
	case EMonsterMontage::ATTACK:
		TempAnimMontage = MonsterData->AttackMontage;
		break;
	case EMonsterMontage::ATTACK_BIG:
		TempAnimMontage = MonsterData->AttackBigMontage;
		break;
	case EMonsterMontage::ATTACK_SPEAR:
		TempAnimMontage = MonsterData->AttackSpearMontage;
		break;
	case EMonsterMontage::ATTACK_SWORD:
		TempAnimMontage = MonsterData->AttackSwordMontage;
		break;
	case EMonsterMontage::ATTACK_LSWORD:
		TempAnimMontage = MonsterData->AttackLSwordMontage;
		break;
	case EMonsterMontage::ATTACK_THROW:
		TempAnimMontage = MonsterData->AttackThrowMontage;
		break;
	case EMonsterMontage::JUMP_START:
		TempAnimMontage = MonsterData->JumpStartMontage;
		break;
	case EMonsterMontage::JUMP_END:
		TempAnimMontage = MonsterData->JumpEndMontage;
		break;
	case EMonsterMontage::DAMAGE:
		TempAnimMontage = MonsterData->DamageMontage;
		break;
	case EMonsterMontage::ANGRY:
		TempAnimMontage = MonsterData->AngryMontage;
		break;
	case EMonsterMontage::BOW_START:
		TempAnimMontage = MonsterData->BowStartMontage;
		break;
	case EMonsterMontage::BOW_END:
		TempAnimMontage = MonsterData->BowEndMontage;
		break;
	case EMonsterMontage::THROW:
		TempAnimMontage = MonsterData->ThrowMontage;
		break;
	case EMonsterMontage::DANCE_START:
		TempAnimMontage = MonsterData->DanceStartMontage;
		break;
	case EMonsterMontage::DANCE_END:
		TempAnimMontage = MonsterData->DanceEndMontage;
		break;
	case EMonsterMontage::WEAPON_CATCH:
		TempAnimMontage = MonsterData->WeaponCatchMontage;
		break;
	case EMonsterMontage::FIND:
		TempAnimMontage = MonsterData->FindMontage;
		break;
	case EMonsterMontage::SIGNAL_START:
		TempAnimMontage = MonsterData->SignalStartMontage;
		break;
	case EMonsterMontage::SIGNAL_END:
		TempAnimMontage = MonsterData->SignalEndMontage;
		break;
	case EMonsterMontage::APPEAR:
		TempAnimMontage = MonsterData->APPEAR;
		break;
	case EMonsterMontage::BOW_UPPER_START:
		TempAnimMontage = MonsterData->BOW_UPPER_START;
		break;
	case EMonsterMontage::BOW_UPPER_END:
		TempAnimMontage = MonsterData->BOW_UPPER_END;
		break;
	case EMonsterMontage::ATTACK_DASH_LSWORD_START:
		TempAnimMontage = MonsterData->ATTACK_DASH_LSWORD_START;
		break;
	case EMonsterMontage::ATTACK_DASH_LSWORD_END:
		TempAnimMontage = MonsterData->ATTACK_DASH_LSWORD_END;
		break;
	case EMonsterMontage::ATTACK_DASH_SWORD_START:
		TempAnimMontage = MonsterData->ATTACK_DASH_SWORD_START;
		break;
	case EMonsterMontage::ATTACK_DASH_SWORD_END:
		TempAnimMontage = MonsterData->ATTACK_DASH_SWORD_END;
		break;
	case EMonsterMontage::ATTACK_EXPLOSION_START:
		TempAnimMontage = MonsterData->ATTACK_EXPLOSION_START;
		break;
	case EMonsterMontage::ATTACK_EXPLOSION_END:
		TempAnimMontage = MonsterData->ATTACK_EXPLOSION_END;
		break;
	case EMonsterMontage::ATTACK_FIRE_START:
		TempAnimMontage = MonsterData->ATTACK_FIRE_START;
		break;
	case EMonsterMontage::ATTACK_FIRE:
		TempAnimMontage = MonsterData->ATTACK_FIRE;
		break;
	case EMonsterMontage::ATTACK_FIRE_END:
		TempAnimMontage = MonsterData->ATTACK_FIRE_END;
		break;
	case EMonsterMontage::ATTACK_HORN_START:
		TempAnimMontage = MonsterData->ATTACK_HORN_START;
		break;
	case EMonsterMontage::ATTACK_HORN_END:
		TempAnimMontage = MonsterData->ATTACK_HORN_END;
		break;
	case EMonsterMontage::ATTACK_RUNNING_LSWORD_START:
		TempAnimMontage = MonsterData->ATTACK_RUNNING_LSWORD_START;
		break;
	case EMonsterMontage::ATTACK_RUNNING_LSWORD_END:
		TempAnimMontage = MonsterData->ATTACK_RUNNING_LSWORD_END;
		break;
	case EMonsterMontage::ATTACK_RUNNING_SWORD_START:
		TempAnimMontage = MonsterData->ATTACK_RUNNING_SWORD_START;
		break;
	case EMonsterMontage::ATTACK_RUNNING_SWORD_END:
		TempAnimMontage = MonsterData->ATTACK_RUNNING_SWORD_END;
		break;
	case EMonsterMontage::REBOUND:
		TempAnimMontage = MonsterData->REBOUND;
		break;
	case EMonsterMontage::RODEO_START:
		TempAnimMontage = MonsterData->RODEO_START;
		break;
	case EMonsterMontage::RODEO_END:
		TempAnimMontage = MonsterData->RODEO_END;
		break;
	case EMonsterMontage::STUN_START:
		TempAnimMontage = MonsterData->STUN_START;
		break;
	case EMonsterMontage::STUN_END:
		TempAnimMontage = MonsterData->STUN_END;
		break;
	case EMonsterMontage::TURN_180_L:
		TempAnimMontage = MonsterData->TURN_180_L;
		break;
	case EMonsterMontage::TURN_180_R:
		TempAnimMontage = MonsterData->TURN_180_R;
		break;
	case EMonsterMontage::DRAW_BOW:
		TempAnimMontage = MonsterData->DRAW_BOW;
		break;
	case EMonsterMontage::SHEATH_BOW:
		TempAnimMontage = MonsterData->SHEATH_BOW;
		break;
	case EMonsterMontage::BOW_TO_SWORD:
		TempAnimMontage = MonsterData->BOW_TO_SWORD;
		break;
	case EMonsterMontage::SWORD_TO_BOW:
		TempAnimMontage = MonsterData->SWORD_TO_BOW;
		break;
	case EMonsterMontage::DRAW_LSWORD:
		TempAnimMontage = MonsterData->DRAW_LSWORD;
		break;
	case EMonsterMontage::SHEATH_LSWORD:
		TempAnimMontage = MonsterData->SHEATH_LSWORD;
		break;
	case EMonsterMontage::GEAR_1_FORWARD:
		TempAnimMontage = MonsterData->Gear_1_Forward;
		break;
	case EMonsterMontage::GEAR_1_LEFT:
		TempAnimMontage = MonsterData->Gear_1_Left;
		break;
	case EMonsterMontage::GEAR_1_RIGHT:
		TempAnimMontage = MonsterData->Gear_1_Right;
		break;
	case EMonsterMontage::GEAR_2_FORWARD:
		TempAnimMontage = MonsterData->Gear_2_Forward;
		break;
	case EMonsterMontage::GEAR_2_LEFT:
		TempAnimMontage = MonsterData->Gear_2_Left;
		break;
	case EMonsterMontage::GEAR_2_RIGHT:
		TempAnimMontage = MonsterData->Gear_2_Right;
		break;
	case EMonsterMontage::GEAR_3_FORWARD:
		TempAnimMontage = MonsterData->Gear_3_Forward;
		break;
	case EMonsterMontage::GEAR_3_LEFT:
		TempAnimMontage = MonsterData->Gear_3_Left;
		//UE_LOG(LogTemp, Warning, TEXT("IsPlayingMontage EMonsterMontage::GEAR_3_LEFT "));
		break;
	case EMonsterMontage::GEAR_3_RIGHT:
		TempAnimMontage = MonsterData->Gear_3_Right;
		//UE_LOG(LogTemp, Warning, TEXT("IsPlayingMontage EMonsterMontage::GEAR_3_RIGHT "));
		break;
	case EMonsterMontage::END:
		TempAnimMontage = nullptr;
		break;
	default:
		break;
	}

	bool bFlag = AnimInstance->Montage_IsPlaying(TempAnimMontage);
	//UE_LOG(LogTemp, Warning, TEXT("IsPlayingMontage : %s"), bFlag ? TEXT("true") : TEXT("false"));
	return bFlag;
}

void IMonsterInterface::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	UMonsterStatusComponent * UMonsterStatusComponent = GetStatusComponent();
	const float fDamage = UMonsterStatusComponent->TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	if (fDamage > 0.f)
	{
		PlayMontage(EMonsterMontage::DAMAGE);
	}
}
