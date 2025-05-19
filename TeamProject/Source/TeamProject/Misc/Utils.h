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
    ToDance,
    Dance,
    End,
};



enum class MONSTER_MONTAGE : uint8
{
    ATTACK = 0,
    ATTACK_BIG,
    ATTACK_SPEAR,
    ATTACK_SWORD,
    ATTACK_LSWORD,
    ATTACK_THROW,
    JUMP_START,
    JUMP_END,
    DAMAGE,
    ANGRY,
    BOW_START,
    BOW_END,
    THROW,
    DANCE_START,
    DANCE_END,
    WEAPON_CATCH,
    FIND,
    END,
};


inline void RotateActorToDirection(AActor* TargetActor, const FVector& TargetDirection)
{
    if (!TargetActor || TargetDirection.IsNearlyZero())
        return;

    // 방향을 회전으로 변환
    FRotator TargetRotation = TargetDirection.Rotation();

    // 회전을 적용
    TargetActor->SetActorRotation(TargetRotation);
}

inline void SmoothRotateActorToDirection(AActor* TargetActor, const FVector& TargetDirection, float DeltaTime, float InterpSpeed = 5.f)
{
    if (!TargetActor || TargetDirection.IsNearlyZero())
        return;

    FRotator CurrentRot = TargetActor->GetActorRotation();
    FRotator TargetRot = TargetDirection.Rotation();

    FRotator NewRot = FMath::RInterpTo(CurrentRot, TargetRot, DeltaTime, InterpSpeed);
    TargetActor->SetActorRotation(NewRot);
}
