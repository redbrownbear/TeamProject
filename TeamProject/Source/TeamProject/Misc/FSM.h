#pragma once
#include "CoreMinimal.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

namespace CollisionProfileName
{
    static inline FName NPC = TEXT("NPC");
    static inline FName NPCProjectile = TEXT("NPCProjectile");
    static inline FName MapMesh = TEXT("MapMesh");
}

UENUM()
enum class ENpcState : uint8
{
    Idle = 0,
    Patrol,
    Talk,
    Hide,
    Play, // 술래잡기: 상황 봐서 여유되면 추가
};