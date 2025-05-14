#pragma once
#include "CoreMinimal.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

namespace CollisionProfileName
{
    static inline FName Monster = TEXT("Korogu");
    static inline FName MonsterProjectile = TEXT("KoroguProjectile");
    static inline FName MapMesh = TEXT("MapMesh");
}

UENUM()
enum class EKoroguState : uint8
{
    Idle = 0,
    Patrol,
    Hide,
};