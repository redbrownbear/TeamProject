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
    static inline FName Player = TEXT("Player");
    static inline FName ToPlayer = TEXT("ToPlayer");
    static inline FName ToMonster = TEXT("ToMonster");
    static inline FName MapMesh = TEXT("MapMesh");
    static inline FName Item = TEXT("Item");
    static inline FName ToItem = TEXT("ToItem");
}

namespace ProjectileName
{
    static inline FName Monster_Attack = TEXT("Monster_Attack");
    static inline FName Monster_PlayerAlert = TEXT("Monster_PlayerAlert");
    static inline FName Monster_CatchItem = TEXT("Monster_CatchItem");
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
    FindWeapon,
    Combat,
    Dead,
    Fire,
    Eat,
    ToDance,
    Dance,
    Signal,
    End,
};

enum class EMonsterMontage : uint8
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
    SIGNAL_START,
    SIGNAL_END,
    END,
};

UENUM()
enum class EWeaponKind : uint8
{
    SWORD = 0,
    SPEAR,
    LSWORD,
    BOW,
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

inline void SmoothRotateActorToDirection(AActor* TargetActor, const FVector& TargetLocation, float DeltaTime, float InterpSpeed = 5.f)
{
    if (!TargetActor)
        return;

    FVector ActorLocation = TargetActor->GetActorLocation();
    FVector Direction = TargetLocation - ActorLocation;
    Direction.Normalize();
    FRotator CurrentRot = TargetActor->GetActorRotation();
    FRotator TargetRot = Direction.Rotation();

    FRotator NewRot = FMath::RInterpTo(CurrentRot, TargetRot, DeltaTime, InterpSpeed);
    TargetActor->SetActorRotation(NewRot);
}
