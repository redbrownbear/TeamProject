// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Monster/MonsterInterface.h"
#include "Actors/Item/WorldWeapon.h"
#include "Actors/Controller/AIController/Monster/MonsterAIController.h"
#include "Actors/Projectile/Projectile.h"
#include "Actors/Effect/NiagaraEffect.h"
#include "Actors/Effect/ParticleEffect.h"
#include "Actors/Monster/CharacterMonster.h"


#include "Components/FSMComponent/Monster/MonsterFSMComponent.h"
#include "Components/FSMComponent/Monster/HinoxFSMComponent.h"
#include "Components/FSMComponent/Monster/LynelFSMComponent.h"
#include "Components/FSMComponent/Monster/AssasinBossFSMComponent.h"
#include "Components/FSMComponent/Monster/AssasinLeaderFSMComponent.h"
#include "Components/StatusComponent/MonsterStatusComponent/MonsterStatusComponent.h"


#include "Data/MonsterTableRow.h"
#include "Data/ItemDataRow.h"
#include "Data/NiagaraEffectTableRow.h"
#include "Data/ParticleEffectTableRow.h"

#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

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
	case EMonsterMontage::DEAD:
		TempAnimMontage = MonsterData->Dead;
		break;
	case EMonsterMontage::FOOT:
		TempAnimMontage = MonsterData->Foot;
		break;
	case EMonsterMontage::HANDCLAP:
		TempAnimMontage = MonsterData->HandClap;
		break;
	case EMonsterMontage::HIPDROP:
		TempAnimMontage = MonsterData->HipDrop;
		break;
	case EMonsterMontage::LEFTHAND:
		TempAnimMontage = MonsterData->LeftHand;
		break;
	case EMonsterMontage::DAMAGE_EYE_START:
		TempAnimMontage = MonsterData->Damage_Eye_Start;
		break;
	case EMonsterMontage::DAMAGE_EYE_END:
		TempAnimMontage = MonsterData->Damage_Eye_End;
		break;
	case EMonsterMontage::DAMAGE_FOOT_L:
		TempAnimMontage = MonsterData->Damage_Foot_L;
		break;
	case EMonsterMontage::DAMAGE_FOOT_R:
		TempAnimMontage = MonsterData->Damage_Foot_R;
		break;
	case EMonsterMontage::RUN:
		TempAnimMontage = MonsterData->Run;
		break;
	case EMonsterMontage::RUN_CURVE_R:
		TempAnimMontage = MonsterData->Run_Curve_R;
		break;
	case EMonsterMontage::RUN_CURVE_L:
		TempAnimMontage = MonsterData->Run_Curve_L;
		break;
	case EMonsterMontage::SLEEP_START:
		TempAnimMontage = MonsterData->Sleep_Start;
		break;
	case EMonsterMontage::SLEEP_END:
		TempAnimMontage = MonsterData->Sleep_End;
		break;
	case EMonsterMontage::THROW_STONE_START:
		TempAnimMontage = MonsterData->Throw_Stone_Start;
		break;
	case EMonsterMontage::THROW_STONE_END:
		TempAnimMontage = MonsterData->Throw_Stone_End;
		break;
	case EMonsterMontage::SEARCH:
		TempAnimMontage = MonsterData->SearchMontage;
		break;
	case EMonsterMontage::APPEAR_START:
		TempAnimMontage = MonsterData->APPEAR_START;
		break;
	case EMonsterMontage::APPEAR_END:
		TempAnimMontage = MonsterData->APPEAR_END;
		break;
	case EMonsterMontage::BARRIER_START:
		TempAnimMontage = MonsterData->BARRIER_START;
		break;
	case EMonsterMontage::BARRIER_END:
		TempAnimMontage = MonsterData->BARRIER_END;
		break;
	case EMonsterMontage::ATTACK_BALL_FIRST_START:
		TempAnimMontage = MonsterData->ATTACK_BALL_FIRST_START;
		break;
	case EMonsterMontage::ATTACK_BALL_FIRST_END:
		TempAnimMontage = MonsterData->ATTACK_BALL_FIRST_END;
		break;
	case EMonsterMontage::ATTACK_BALL_SECOND_START:
		TempAnimMontage = MonsterData->ATTACK_BALL_SECOND_START;
		break;
	case EMonsterMontage::ATTACK_BALL_SECOND_END:
		TempAnimMontage = MonsterData->ATTACK_BALL_SECOND_END;
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
	else if (TempAnimMontage == MonsterData->REBOUND)
	{
		AnimInstance->Montage_Play(TempAnimMontage);
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
	case EMonsterMontage::DEAD:
		TempAnimMontage = MonsterData->Dead;
		break;
	case EMonsterMontage::FOOT:
		TempAnimMontage = MonsterData->Foot;
		break;
	case EMonsterMontage::HANDCLAP:
		TempAnimMontage = MonsterData->HandClap;
		break;
	case EMonsterMontage::HIPDROP:
		TempAnimMontage = MonsterData->HipDrop;
		break;
	case EMonsterMontage::LEFTHAND:
		TempAnimMontage = MonsterData->LeftHand;
		break;
	case EMonsterMontage::DAMAGE_EYE_START:
		TempAnimMontage = MonsterData->Damage_Eye_Start;
		break;
	case EMonsterMontage::DAMAGE_EYE_END:
		TempAnimMontage = MonsterData->Damage_Eye_End;
		break;
	case EMonsterMontage::DAMAGE_FOOT_L:
		TempAnimMontage = MonsterData->Damage_Foot_L;
		break;
	case EMonsterMontage::DAMAGE_FOOT_R:
		TempAnimMontage = MonsterData->Damage_Foot_R;
		break;
	case EMonsterMontage::RUN:
		TempAnimMontage = MonsterData->Run;
		break;
	case EMonsterMontage::RUN_CURVE_R:
		TempAnimMontage = MonsterData->Run_Curve_R;
		break;
	case EMonsterMontage::RUN_CURVE_L:
		TempAnimMontage = MonsterData->Run_Curve_L;
		break;
	case EMonsterMontage::SLEEP_START:
		TempAnimMontage = MonsterData->Sleep_Start;
		break;
	case EMonsterMontage::SLEEP_END:
		TempAnimMontage = MonsterData->Sleep_End;
		break;
	case EMonsterMontage::THROW_STONE_START:
		TempAnimMontage = MonsterData->Throw_Stone_Start;
		break;
	case EMonsterMontage::THROW_STONE_END:
		TempAnimMontage = MonsterData->Throw_Stone_End;
		break;
	case EMonsterMontage::SEARCH:
		TempAnimMontage = MonsterData->SearchMontage;
		break;
	case EMonsterMontage::APPEAR_START:
		TempAnimMontage = MonsterData->APPEAR_START;
		break;
	case EMonsterMontage::APPEAR_END:
		TempAnimMontage = MonsterData->APPEAR_END;
		break;
	case EMonsterMontage::BARRIER_START:
		TempAnimMontage = MonsterData->BARRIER_START;
		break;
	case EMonsterMontage::BARRIER_END:
		TempAnimMontage = MonsterData->BARRIER_END;
		break;
	case EMonsterMontage::ATTACK_BALL_FIRST_START:
		TempAnimMontage = MonsterData->ATTACK_BALL_FIRST_START;
		break;
	case EMonsterMontage::ATTACK_BALL_FIRST_END:
		TempAnimMontage = MonsterData->ATTACK_BALL_FIRST_END;
		break;
	case EMonsterMontage::ATTACK_BALL_SECOND_START:
		TempAnimMontage = MonsterData->ATTACK_BALL_SECOND_START;
		break;
	case EMonsterMontage::ATTACK_BALL_SECOND_END:
		TempAnimMontage = MonsterData->ATTACK_BALL_SECOND_END;
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
	case EMonsterMontage::DEAD:
		TempAnimMontage = MonsterData->Dead;
		break;
	case EMonsterMontage::FOOT:
		TempAnimMontage = MonsterData->Foot;
		break;
	case EMonsterMontage::HANDCLAP:
		TempAnimMontage = MonsterData->HandClap;
		break;
	case EMonsterMontage::HIPDROP:
		TempAnimMontage = MonsterData->HipDrop;
		break;
	case EMonsterMontage::LEFTHAND:
		TempAnimMontage = MonsterData->LeftHand;
		break;
	case EMonsterMontage::DAMAGE_EYE_START:
		TempAnimMontage = MonsterData->Damage_Eye_Start;
		break;
	case EMonsterMontage::DAMAGE_EYE_END:
		TempAnimMontage = MonsterData->Damage_Eye_End;
		break;
	case EMonsterMontage::DAMAGE_FOOT_L:
		TempAnimMontage = MonsterData->Damage_Foot_L;
		break;
	case EMonsterMontage::DAMAGE_FOOT_R:
		TempAnimMontage = MonsterData->Damage_Foot_R;
		break;
	case EMonsterMontage::RUN:
		TempAnimMontage = MonsterData->Run;
		break;
	case EMonsterMontage::RUN_CURVE_R:
		TempAnimMontage = MonsterData->Run_Curve_R;
		break;
	case EMonsterMontage::RUN_CURVE_L:
		TempAnimMontage = MonsterData->Run_Curve_L;
		break;
	case EMonsterMontage::SLEEP_START:
		TempAnimMontage = MonsterData->Sleep_Start;
		break;
	case EMonsterMontage::SLEEP_END:
		TempAnimMontage = MonsterData->Sleep_End;
		break;
	case EMonsterMontage::THROW_STONE_START:
		TempAnimMontage = MonsterData->Throw_Stone_Start;
		break;
	case EMonsterMontage::THROW_STONE_END:
		TempAnimMontage = MonsterData->Throw_Stone_End;
		break;
	case EMonsterMontage::SEARCH:
		TempAnimMontage = MonsterData->SearchMontage;
		break;
	case EMonsterMontage::APPEAR_START:
		TempAnimMontage = MonsterData->APPEAR_START;
		break;
	case EMonsterMontage::APPEAR_END:
		TempAnimMontage = MonsterData->APPEAR_END;
		break;
	case EMonsterMontage::BARRIER_START:
		TempAnimMontage = MonsterData->BARRIER_START;
		break;
	case EMonsterMontage::BARRIER_END:
		TempAnimMontage = MonsterData->BARRIER_END;
		break;
	case EMonsterMontage::ATTACK_BALL_FIRST_START:
		TempAnimMontage = MonsterData->ATTACK_BALL_FIRST_START;
		break;
	case EMonsterMontage::ATTACK_BALL_FIRST_END:
		TempAnimMontage = MonsterData->ATTACK_BALL_FIRST_END;
		break;
	case EMonsterMontage::ATTACK_BALL_SECOND_START:
		TempAnimMontage = MonsterData->ATTACK_BALL_SECOND_START;
		break;
	case EMonsterMontage::ATTACK_BALL_SECOND_END:
		TempAnimMontage = MonsterData->ATTACK_BALL_SECOND_END;
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

void IMonsterInterface::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser, int32 iOption)
{
	if (FMath::IsNearlyZero(Damage)) return;

	AActor* ThisActor = Cast<AActor>(this);

	UMonsterStatusComponent * UMonsterStatusComponent = GetStatusComponent();
	float fDamage = UMonsterStatusComponent->TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	if (UMonsterFSMComponent* FSMComponent = GetFSMComponent())
	{
		if (fDamage > 0.f)
		{
			// for UAssasinBossFSMComponent
			if (Cast<AActor>(this) == DamageCauser && FSMComponent->IsA<UAssasinBossFSMComponent>())
			{
				FSMComponent->ChangeState(EMonsterState::Stun);
			}
			// Normal Situation
			else
			{
				// check if it is a head shot
				if (iOption)
				{
					if (FSMComponent->IsA<UHinoxFSMComponent>())
					{
						FSMComponent->ChangeState(EMonsterState::Damage_Eye);
					}
					else if (FSMComponent->IsA<ULynelFSMComponent>())
					{
						FSMComponent->ChangeState(EMonsterState::Stun);
					}
				}
				else
				{
					if (FSMComponent->IsA<UAssasinLeaderFSMComponent>())
					{
						if (FSMComponent->GetCurrentState() == EMonsterState::Patrol)
						{
							FVector PlayerLook = DamageCauser->GetActorForwardVector();
							FVector MonsterLook = ThisActor->GetActorForwardVector();
							PlayerLook.Z = 0.f;
							MonsterLook.Z = 0.f;
							PlayerLook.Normalize();
							MonsterLook.Normalize();
							float Dot = FVector::DotProduct(PlayerLook, MonsterLook);

							FSMComponent->ChangeState(EMonsterState::Damage);

							if (Dot > 0.8)
							{
								FTimerHandle TimerHandle;
								ThisActor->GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this, FSMComponent]()
									{
										FSMComponent->ChangeState(EMonsterState::Dead);
									}, 0.2f, false);
							}
						}
						// Combat
						else
						{
							if (ACharacterMonster* CharacterMonster = Cast<ACharacterMonster>(ThisActor))
							{
								if (!CharacterMonster->IsPlayingMontage(EMonsterMontage::REBOUND)
									&& CharacterMonster->IsPlayingMontage())
								{

									FSMComponent->ChangeState(EMonsterState::Damage);
								}
								else
								{
									// Recover Damage as AssasinLeader Defended attack
									fDamage += UMonsterStatusComponent->TakeDamage(-Damage, DamageEvent, EventInstigator, DamageCauser);
									FSMComponent->ChangeState(EMonsterState::Rebound);
								}
							}
							else
							{
								UE_LOG(LogTemp, Error, TEXT("MonsterInterface::TakeDamage // No CharacterMonster"));
								check(false);
							}
						}
					}
					else
					{
						FSMComponent->ChangeState(EMonsterState::Damage);
					}
				}
			}
		}
		else if (FMath::IsNearlyZero(fDamage))
		{
			FSMComponent->ChangeState(EMonsterState::Dead);
		}
	}

	if (!FMath::IsNearlyZero(fDamage))
	{
		AddBaseColor(FVector(1.f, -0.3f, -0.3f));
	}

	FVector ZeroVector = FVector::Zero();
	FTimerHandle TimerHandle;


	ThisActor->GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this, ZeroVector]()
		{
			AddBaseColor(ZeroVector);
		}, 0.5f, false);
	
}

void IMonsterInterface::OnDie()
{
	GetFSMComponent()->ChangeState(EMonsterState::Dead);
}

void IMonsterInterface::OnDeadEnd()
{
	FMonsterTableRow* MonsterData = GetMonsterData();
	UObject* Object = Cast<UObject>(this);


	const FDataTableRowHandle ParticleEffectDataTable = MonsterData->ParticleEffectTableRowHandle;
	if (AActor* Actor = Cast<AActor>(Object))
	{
		if (!ParticleEffectDataTable.IsNull())
		{
			{
				UWorld* World = Actor->GetWorld();

				AParticleEffect* Effect = World->SpawnActorDeferred<AParticleEffect>(AParticleEffect::StaticClass(),
					FTransform::Identity, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

				FTransform NewTransform;
				Effect->SetData(MonsterData->ParticleEffectTableRowHandle);
				NewTransform.SetLocation(Actor->GetActorLocation());
				NewTransform.SetRotation(FRotator::ZeroRotator.Quaternion());
				Effect->FinishSpawning(NewTransform);
			}
		}

		const FDataTableRowHandle NiagaraEffectDataTable = MonsterData->NiagaraEffectTableRowHandle;
		if (!NiagaraEffectDataTable.IsNull())
		{
			UWorld* World = Actor->GetWorld();

			ANiagaraEffect* Effect = World->SpawnActorDeferred<ANiagaraEffect>(ANiagaraEffect::StaticClass(),
				FTransform::Identity, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

			FTransform NewTransform;
			Effect->SetData(MonsterData->ParticleEffectTableRowHandle);
			NewTransform.SetLocation(Actor->GetActorLocation());
			NewTransform.SetRotation(FRotator::ZeroRotator.Quaternion());
			Effect->FinishSpawning(NewTransform);
		}
	}
}

FName IMonsterInterface::GetName() const
{
	if (FMonsterTableRow* MonsterData = GetMonsterData())
	{
		return MonsterData->Name;
	}
	return FName(TEXT(""));
}

float IMonsterInterface::GetDamageFromWeapon()
{
	UMonsterFSMComponent* FSMComponent = GetFSMComponent();
	if (const AWorldWeapon* WW = FSMComponent->GetCurrentWeapon())
	{
		float fDamage = WW->GetDamage();
		return fDamage;
	}
	return 0.0f;
}
