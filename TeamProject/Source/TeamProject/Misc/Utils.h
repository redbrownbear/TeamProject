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
    static inline FName Player = TEXT("Player");
    static inline FName ToPlayer = TEXT("ToPlayer");
	static inline FName Monster = TEXT("Monster");
    static inline FName ToMonster = TEXT("ToMonster");
    static inline FName MapMesh = TEXT("MapMesh");
    static inline FName Item = TEXT("Item");
    static inline FName ToItem = TEXT("ToItem");
    static inline FName NPC = TEXT("NPC");
    static inline FName ToNPC = TEXT("ToNPC");
}

namespace ProjectileName
{
    static inline FName Monster_Attack = TEXT("Monster_Attack");
    static inline FName Monster_PlayerAlert = TEXT("Monster_PlayerAlert");
    static inline FName Monster_CatchItem = TEXT("Monster_CatchItem");
    static inline FName Monster_Arrow = TEXT("Monster_Arrow");
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
    AimingBow,
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
enum class EItemCategoryType : int8
{
    IT_Weapon,
    IT_Arrow,
    IT_Shield,
    IT_Armor,
    IT_Material,
    IT_Food,
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



inline void InstantRotateActorToDirection(AActor* TargetActor, const FVector& TargetLocation)
{
    if (!TargetActor)
        return;

    FVector ActorLocation = TargetActor->GetActorLocation();
    FVector Direction = TargetLocation - ActorLocation;

    // 방향 벡터가 0이 아니도록 확인 (같은 위치에 있을 경우 문제 방지)
    if (Direction.IsNearlyZero())
    {
        return; // 대상과 액터가 같은 위치에 있으면 회전할 방향이 없으므로 종료
    }

    Direction.Normalize(); // 방향 벡터 정규화

    // 대상 위치를 바라보는 회전값 계산
    FRotator TargetRot = Direction.Rotation();

    // 액터의 회전을 즉시 변경
    TargetActor->SetActorRotation(TargetRot);
}

inline void SmoothRotateActorToDirection(AActor* TargetActor, const FVector& TargetLocation, float DeltaTime, float InterpSpeed = 5.f)
{
    if (!TargetActor)
        return;

    FVector ActorLocation = TargetActor->GetActorLocation();
    FVector Direction = TargetLocation - ActorLocation;
    Direction.Z = 0.f;
    Direction.Normalize();
    FRotator CurrentRot = TargetActor->GetActorRotation();
    FRotator TargetRot = Direction.Rotation();

    FRotator NewRot = FMath::RInterpTo(CurrentRot, TargetRot, DeltaTime, InterpSpeed);
    TargetActor->SetActorRotation(NewRot);
}

UENUM()
enum class EWeapon_Type
{
    Sword,
    Shield,
    Bow
};

UENUM()
enum class EEquip_State
{
    None,
    Sword,
    Shield,
    Sword_Shield,
    Bow,

};

// NPC 추가
UENUM()
enum class ENpcState : uint8
{
    Idle = 0,
    Sit,
    Stand,
    Walk,
    Run,
    Talk,
    Hide, // 술래잡기: 시간 되면 추가
    Sell,
    End,
};


enum class ENpcMontage : uint8
{
    IDLE = 0,
    SIT,
    STAND,
    WALK,
    RUN,
    TALK,
    HIDE,
    SELL,
    END,
};