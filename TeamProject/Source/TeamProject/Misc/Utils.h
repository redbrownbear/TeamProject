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
    static inline FName ToPlayerMonster = TEXT("ToPlayerMonster");
    static inline FName MapMesh = TEXT("MapMesh");
    static inline FName Item = TEXT("Item");
    static inline FName ToItem = TEXT("ToItem");
    static inline FName NPC = TEXT("NPC");
    static inline FName ToNPC = TEXT("ToNPC");
    static inline FName Trigger = TEXT("Trigger");
    static inline FName Floating = TEXT("Floating");
    static inline FName Water = TEXT("Water");
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

    static inline FName Monster_HinoxAttack = TEXT("Monster_HinoxAttack");
    static inline FName Monster_HinoxGrab = TEXT("Monster_HinoxGrab");
    static inline FName Monster_HinoxStone = TEXT("Monster_HinoxStone");
    static inline FName Monster_HinoxLink = TEXT("Monster_HinoxLink");
    static inline FName Monster_HinoxHipDrop = TEXT("Monster_HinoxHipDrop");

    static inline FName Monster_AL_Attack = TEXT("Monster_AL_Attack");
    static inline FName Monster_AL_AttackBig = TEXT("Monster_AL_AttackBig");

    static inline FName Monster_AB_KogaStone = TEXT("Monster_AB_KogaStone");
    static inline FName Monster_AB_KogaStoneBig = TEXT("Monster_AB_KogaStoneBig");

    static inline FName Player_Arrow = TEXT("Player_Arrow");
    static inline FName Player_FireArrow = TEXT("Player_FireArrow");
}

namespace MonsterName
{
    static inline FName Bokoblin_TreasureBox = TEXT("Bokoblin_TreasureBox");
    static inline FName Bokoblin_Patrol = TEXT("Bokoblin_Patrol");
    static inline FName Moriblin_TreasureBox = TEXT("Moriblin_TreasureBox");
    static inline FName Moriblin_Patrol = TEXT("Moriblin_Patrol");
    static inline FName Hinox = TEXT("Hinox");
    static inline FName Lynel = TEXT("Lynel");
    static inline FName AssasinBoss = TEXT("AssasinBoss");
    static inline FName AssasinLeader = TEXT("AssasinLeader");
}

namespace ParticleEffectName
{
    static inline FName MonsterDeath = TEXT("MonsterDeath");
    static inline FName UnopenedBox = TEXT("UnopenedBox");
    static inline FName Hinox_AttackShockWave = TEXT("Hinox_AttackShockWave");
    static inline FName Hinox_HipdropShockWave = TEXT("Hinox_HipdropShockWave");
    static inline FName Lynel_Explosion = TEXT("Lynel_Explosion");
    static inline FName Lynel_Cry = TEXT("Lynel_Cry");
    static inline FName Lynel_BeforeExplosion = TEXT("Lynel_BeforeExplosion");
}

namespace NiagaraEffectName
{
    static inline FName AL_Attack = TEXT("AL_Attack");
    static inline FName AL_AttackBig = TEXT("AL_AttackBig");
    static inline FName Lynel_Attack = TEXT("Lynel_Attack");
}

namespace Monster_SocketName
{
    //static inline FName Weapon_R = TEXT("Weapon_R");
    //static inline FName Weapon_L = TEXT("Weapon_L");
    static inline FName Weapon_Right = TEXT("Weapon_Right");
    static inline FName Weapon_Left = TEXT("Weapon_Left");
    static inline FName Pod_A = TEXT("Pod_A");
    static inline FName Pod_B = TEXT("Pod_B");
    static inline FName Pod_C = TEXT("Pod_C");
    static inline FName Pod_D = TEXT("Pod_D");
    static inline FName Pod_Melee = TEXT("Pod_Melee");
    static inline FName Pod_Bow = TEXT("Pod_Bow");
    static inline FName Chin = TEXT("Chin");
    static inline FName Center_1 = TEXT("Center_1");
    static inline FName Center_2 = TEXT("Center_2");
    static inline FName Leg_1_R = TEXT("Leg_1_R");
    static inline FName Leg_2_R = TEXT("Leg_2_R");
    static inline FName Leg_1_L = TEXT("Leg_1_L");
    static inline FName Leg_2_L = TEXT("Leg_2_L");
    static inline FName Toe_R = TEXT("Toe_R");
    static inline FName Toe_L = TEXT("Toe_L");
    static inline FName EyeBall = TEXT("EyeBall");
}

enum class EAdditionalCollider : uint8
{
    Chin = 0,
    Center_1,
    Center_2,
    Leg_1_R,
    Leg_2_R,
    Leg_1_L,
    Leg_2_L,
    Toe_R,
    Toe_L,
    Eye_Ball,
    End
};

namespace ThrownObject
{
    static inline FName HinoxRock = TEXT("HinoxRock");
    static inline FName SmallRock = TEXT("SmallRock");
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
    ReadyToAttack,
    Damage_Eye,
    Happy,
    Barrier,
    Stone,
    Damage,
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
    GEAR_1_FORWARD,
    GEAR_1_LEFT,
    GEAR_1_RIGHT,
    GEAR_2_FORWARD,
    GEAR_2_LEFT,
    GEAR_2_RIGHT,
    GEAR_3_FORWARD,
    GEAR_3_LEFT,
    GEAR_3_RIGHT,
    DEAD,
    FOOT,
	HANDCLAP,
	HIPDROP,
    LEFTHAND,
	DAMAGE_EYE_START,
	DAMAGE_EYE_END,
    DAMAGE_FOOT_L,
	DAMAGE_FOOT_R,
    RUN,
    RUN_CURVE_R,
	RUN_CURVE_L,
	SLEEP_START,
	SLEEP_END,
	THROW_STONE_START,
	THROW_STONE_END,

    APPEAR_START,
    APPEAR_END,
    BARRIER_START,
    BARRIER_END,
    ATTACK_BALL_FIRST_START,
    ATTACK_BALL_FIRST_END,
    ATTACK_BALL_SECOND_START,
    ATTACK_BALL_SECOND_END,


    END,
};

UENUM()
enum class ELynelCombatIndex : uint8
{
    AimingBow = 0, 
    HornAttack, 
    DashAttack, 
    RunningAttack,
    ExplosionAttack, 
    FireAttack, 
    AimingBowUpper, 
    End
};

UENUM()
enum class EHinoxCombatIndex : uint8
{
    Foot = 0,
    HandClap,
    LeftHand,
    Hipdrop,
    ThrowStone,
    End
};

UENUM()
enum class EAssasinBossPhase : uint8
{
    PHASE_1 = 0,
    PHASE_2,
    PHASE_3,
    END,
};

UENUM()
enum class EAssasinBossCombat : uint8
{
    BARRIER = 0,
    STONE,
    END,
};

UENUM()
enum class EReadyToAttackStep : uint8
{
    RunToLink = 0,
    TurnRight,
    AwayFromLink,
    TurnLeft,
    End
};


UENUM()
enum class EItemCategory : int8
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
    None = 0,
    SWORD,
    SPEAR,
    LSWORD,
    BOW,
    END,
};

UENUM()
enum class EKogaStoneKind : uint8
{
    FIRST = 0,
    SECOND,
    THIRD,
    END
};

UENUM()
enum class EArmorKind : uint8
{
    None = 0,
    HEAD,
    ARMOR,
    LEG,
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
UENUM()
enum class EMove_State
{
    Run,
    Glide,
    Climb,
    Dash,
    Zoom,
    Crouch,
    Step,
    BackFlip,
    Hit
};

UENUM()
enum class EClimb_State
{
    Climb,
    Land,
};


// NPC 추가
UENUM()
enum class ENpcState : uint8
{
    Idle = 0,
    Sit,
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