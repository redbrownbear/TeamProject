#pragma once
#include "CoreMinimal.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Defines.h"

namespace CollisionProfileName
{
	static inline FName Monster = TEXT("Monster");
	static inline FName MonsterProjectile = TEXT("MonsterProjectile");
	static inline FName MapMesh = TEXT("MapMesh");
}


UENUM()
enum class EMonsterGroupType : uint8
{
    Patrol = 0,
    TreasureBox,
    Alone,
    End,
};

UENUM()
enum class EMonsterState : uint8
{
    Idle = 0,
    Patrol,
    Suspicious,
    Alert,
    Combat,
    Dead,
    Fire,
    Eat,
    Dance,
    End,
};