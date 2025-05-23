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
    static inline FName Monster_Arrow = TEXT("Monster_Arrow");
    static inline FName Monster_LynelAttack = TEXT("Monster_LynelAttack");
    static inline FName Monster_LynelExplosion = TEXT("Monster_LynelExplosion");
    static inline FName Monster_LynelFire = TEXT("Monster_LynelFire");
    static inline FName Monster_LynelArrow = TEXT("Monster_LynelArrow");
}

namespace Monster_SocketName
{
    static inline FName Weapon_R = TEXT("Weapon_R");
    static inline FName Weapon_L = TEXT("Weapon_L");
    static inline FName Pod_A = TEXT("Pod_A");
    static inline FName Pod_B = TEXT("Pod_B");
    static inline FName Pod_C = TEXT("Pod_C");
    static inline FName Pod_D = TEXT("Pod_D");
    static inline FName Pod_Melee = TEXT("Pod_B");
    static inline FName Pod_Bow = TEXT("Pod_C");
    static inline FName Chin = TEXT("Chin");
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
    AimingBowUpper,
    DashAttack,
    ExplosionAttack,
    FireAttack,
    HornAttack,
    RunningAttack,
    Rebound,
    Rodeo,
    Stun,
    Temp,
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
    SEARCH,
    DANCE_START,
    DANCE_END,
    WEAPON_CATCH,
    FIND,
    SIGNAL_START,
    SIGNAL_END,
    APPEAR,
    BOW_UPPER_START,
    BOW_UPPER_END,
    ATTACK_DASH_LSWORD_START,
    ATTACK_DASH_LSWORD_END,
    ATTACK_DASH_SWORD_START,
    ATTACK_DASH_SWORD_END,
    ATTACK_EXPLOSION_START,
    ATTACK_EXPLOSION_END,
    ATTACK_FIRE_START,
    ATTACK_FIRE,
    ATTACK_FIRE_END,
    ATTACK_HORN_START,
    ATTACK_HORN_END,
    ATTACK_RUNNING_LSWORD_START,
    ATTACK_RUNNING_LSWORD_END,
    ATTACK_RUNNING_SWORD_START,
    ATTACK_RUNNING_SWORD_END,
    REBOUND,
    RODEO_START,
    RODEO_END,
    STUN_START,
    STUN_END,
    TURN_180_L,
    TURN_180_R,
    DRAW_BOW,
    SHEATH_BOW,
    BOW_TO_SWORD,
    SWORD_TO_BOW,
    DRAW_LSWORD,
    SHEATH_LSWORD,
    END,
};

UENUM()
enum class ECombatIndex : uint8
{
    AimingBow = 0, 
    DashAttack, 
    ExplosionAttack, 
    FireAttack, 
    AimingBowUpper, 
    HornAttack, 
    RunningAttack,
    End
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

inline float GetSideOfActor(AActor* A_Actor, AActor* B_Actor)
{
    if (!A_Actor || !B_Actor)
    {
        UE_LOG(LogTemp, Error, TEXT("GetSideOfActor // A_Actor or B_Actor is null"));
        return 0.f;
    }

    FVector A_ForwardVector = A_Actor->GetActorForwardVector();
    A_ForwardVector.Z = 0.f;
    A_ForwardVector.Normalize();

    FVector A_To_B_Vector = B_Actor->GetActorLocation() - A_Actor->GetActorLocation();
    A_To_B_Vector.Z = 0.f;
    A_To_B_Vector.Normalize();

    FVector A_RightVector = A_Actor->GetActorRightVector();
    A_RightVector.Z = 0.f;
    A_RightVector.Normalize();

    float DotProductResult = FVector::DotProduct(A_To_B_Vector, A_RightVector);

    return DotProductResult;
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