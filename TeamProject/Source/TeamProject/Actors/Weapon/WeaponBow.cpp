// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Weapon/WeaponBow.h"
#include "Actors/Character/PlayerCharacter.h"
#include "Actors/Projectile/Arrow/Projectile_Arrow.h"
#include "Animation/AnimInstance/PlayerAnimInstance.h"

AWeaponBow::AWeaponBow()
{
    {

        PrimaryActorTick.bCanEverTick = true;

        DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
        RootComponent = DefaultSceneRoot;


        ConstructorHelpers::FObjectFinder<USkeletalMesh> Asset(TEXT("/Script/Engine.SkeletalMesh'/Game/Resources/Player/Bow/Weapon_Bow_001.Weapon_Bow_001'"));

        SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
        SkeletalMeshComponent->SetupAttachment(RootComponent);
        SkeletalMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        SkeletalMeshComponent->SetSkeletalMesh(Asset.Object);
    }

    {
        ConstructorHelpers::FObjectFinder<UAnimMontage> Asset(TEXT("/Script/Engine.AnimMontage'/Game/Resources/Player/Bow/Animation/Equip_Bow_On_Natural_Montage.Equip_Bow_On_Natural_Montage'"));

        if (Asset.Object)
        {
            EquipMontage = Asset.Object;
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("No Anim_Montage"));
        }
    }

    {
        ConstructorHelpers::FObjectFinder<UAnimMontage> Asset(TEXT("/Script/Engine.AnimMontage'/Game/Resources/Player/Bow/Animation/Equip_Bow_Off_Natural_Montage.Equip_Bow_Off_Natural_Montage'"));

        if (Asset.Object)
        {
            UnEquipMontage = Asset.Object;

        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("No Anim_Montage"));
        }
    }
    {
        ConstructorHelpers::FObjectFinder<UAnimMontage> Asset(TEXT("/Script/Engine.AnimMontage'/Game/Resources/Player/Bow/Animation/Bow_Load_Charge_Montage.Bow_Load_Charge_Montage'"));

        if (Asset.Object)
        {
            Attack_MTG = Asset.Object;
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("No Anim_Montage"));
        }
    }
    {
        ArrowClass = AProjectile_Arrow::StaticClass();
    }
}

void AWeaponBow::LeftClickAction()
{
    APlayerCharacter* Player_C = Cast<APlayerCharacter>(GetOwner());

    UPlayerAnimInstance* AnimInst = Cast<UPlayerAnimInstance>(Player_C->GetMesh()->GetAnimInstance());



    Player_C->GetWeaponManagerComponent()->SetCanShot(false);

    AnimInst->bIsZoom = false;
    
    AnimInst->Montage_Resume(Attack_MTG);

    Player_C->GetCharacterMovement()->MaxWalkSpeed = 600.f;


    Player_C->bUseControllerRotationYaw = false; // 컨트롤러 Yaw 방향을 따라 캐릭터 회전


    // 이동 방향으로 자동 회전 비활성화
    Player_C->GetCharacterMovement()->bOrientRotationToMovement = true;

    USpringArmComponent* C_SpringArm = Player_C->GetSpringArm();
  
    FireArrow();
    

    Player_C->ZoomOut();
}

void AWeaponBow::RightClickAction()
{
    
    APlayerCharacter* Player_C = Cast<APlayerCharacter>(GetOwner());


    UPlayerAnimInstance* AnimInst = Cast<UPlayerAnimInstance>(Player_C->GetMesh()->GetAnimInstance());

    if (AnimInst->Montage_IsPlaying(EquipMontage))
    {
        return;
    }
    
    if (AnimInst->bIsZoom)
    {
        return;
    }

    AnimInst->bIsZoom = true;
    

    AnimInst->Montage_Play(Attack_MTG);
    UCharacterMovementComponent* C_Movement = Player_C->GetCharacterMovement();

    C_Movement->MaxWalkSpeed = PLAYER_MOVE_BOW_ZOOM;

    Player_C->bUseControllerRotationYaw = true; // 컨트롤러 Yaw 방향을 따라 캐릭터 회전

    // 이동 방향으로 자동 회전 비활성화
    C_Movement->bOrientRotationToMovement = false;

    USpringArmComponent* C_SpringArm = Player_C->GetSpringArm();

        


    Player_C->ZoomIn();
    
}

void AWeaponBow::FireArrow()
{

    FName SocketName = "String_R_2";


    // 소켓 위치 및 회전 가져오기

    APlayerCharacter* Player_C = Cast<APlayerCharacter>(GetOwner());

    FVector SpawnLocation = SkeletalMeshComponent->GetSocketLocation(SocketName);

    // 조준 방향 계산: 예) 카메라 방향, 또는 컨트롤러 방향
    FVector AimDirection = Player_C->GetControlRotation().Vector(); // 또는 캐릭터 카메라 방향

    // 조준 방향을 회전으로 변환
    FRotator SpawnRotation = AimDirection.Rotation();

    // 스폰 파라미터
    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    SpawnParams.Owner = this;

    // 화살 액터 스폰
    AProjectile_Arrow* Arrow = GetWorld()->SpawnActor<AProjectile_Arrow>(
        ArrowClass,
        SpawnLocation,
        SpawnRotation,
        SpawnParams
    );

    // 방향 설정 (ProjectileMovementComponent가 필요)
    if (Arrow)
    {

    }
}
