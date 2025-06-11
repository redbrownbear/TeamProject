// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Weapon/WeaponBow.h"
#include "Actors/Character/PlayerCharacter.h"
#include "Actors/Projectile/Arrow/Projectile_Arrow.h"
#include "Animation/AnimInstance/PlayerAnimInstance.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "SubSystem/PlayerManager.h"
#include "UI/HUD/MainHUD.h"
#include "Components/Character/PlayerMovementComponent.h"

AWeaponBow::AWeaponBow()
{
    {

        PrimaryActorTick.bCanEverTick = true;

        DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
        RootComponent = DefaultSceneRoot;


        ConstructorHelpers::FObjectFinder<UStaticMesh> Asset(TEXT("/Script/Engine.StaticMesh'/Game/Resources/Weapon/BowStaticMesh/Bow001.Bow001'"));

        StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SkeletalMeshComponent"));
        StaticMeshComponent->SetupAttachment(RootComponent);
        StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        StaticMeshComponent->SetStaticMesh(Asset.Object);

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
            ChargingMTG = Asset.Object;
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("No Anim_Montage"));
        }
    }
    {
        ConstructorHelpers::FObjectFinder<UAnimMontage> Asset(TEXT("/Script/Engine.AnimMontage'/Game/Resources/Player/Bow/Animation/Bow_Attack_Shoot.Bow_Attack_Shoot'"));

        if (Asset.Object)
        {
            ShootMTG = Asset.Object;
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

void AWeaponBow::BeginPlay()
{
    Super::BeginPlay();


}

void AWeaponBow::LeftClickAction()
{
    APlayerCharacter* Player_C = Cast<APlayerCharacter>(GetOwner());

    UPlayerAnimInstance* AnimInst = Cast<UPlayerAnimInstance>(Player_C->GetMesh()->GetAnimInstance());



    if (AnimInst->Montage_IsPlaying(ChargingMTG))
    {

        AnimInst->Montage_Stop(0.f);
        AnimInst->Montage_Play(ShootMTG);
        AnimInst->bIsWaitShield = false;

    }
    Player_C->GetCharacterMovement()->SetMovementMode(MOVE_None);

    Player_C->GetWeaponManagerComponent()->SetCanShot(false);

    AnimInst->bIsZoom = false;

    Player_C->bUseControllerRotationYaw = false; // 컨트롤러 Yaw 방향을 따라 캐릭터 회전


    // 이동 방향으로 자동 회전 비활성화
    UCharacterMovementComponent* C_Movement = Player_C->GetCharacterMovement();
    Player_C->GetCharacterMovement()->bOrientRotationToMovement = true;

    C_Movement->MaxWalkSpeed = PLAYER_MOVE_NML;
    
    Cast<UPlayerMovementComponent>(C_Movement)->SetMoveState(EMove_State::Run);

    Cast<AMainHUD>(GetWorld()->GetFirstPlayerController()->GetHUD())->ShowBowAimgUI(false, 0);

    FireArrow();

    Player_C->ZoomOut();
}

void AWeaponBow::RightClickAction()
{
    
    APlayerCharacter* Player_C = Cast<APlayerCharacter>(GetOwner());


    UPlayerAnimInstance* AnimInst = Cast<UPlayerAnimInstance>(Player_C->GetMesh()->GetAnimInstance());

    UPlayerMovementComponent* Movement = Cast<UPlayerMovementComponent>(Player_C->GetCharacterMovement());

    Movement->MaxWalkSpeed = PLAYER_MOVE_BOW_ZOOM;

    Movement->SetMoveState(EMove_State::Zoom);

    UPlayerManager* PlayerManager = GetGameInstance()->GetSubsystem<UPlayerManager>();
    PlayerManager->SetStaminaUSe(false);

    Movement->bOrientRotationToMovement = false;

    if (AnimInst->Montage_IsPlaying(EquipMontage))
    {
        return;
    }
    
    if (AnimInst->bIsZoom)
    {
        return;
    }

    AnimInst->bIsZoom = true;
    

    AnimInst->Montage_Play(ChargingMTG);
    UCharacterMovementComponent* C_Movement = Player_C->GetCharacterMovement();

    Cast<AMainHUD>(GetWorld()->GetFirstPlayerController()->GetHUD())->ShowBowAimgUI(true, 0);

    Player_C->bUseControllerRotationYaw = true; // 컨트롤러 Yaw 방향을 따라 캐릭터 회전



    USpringArmComponent* C_SpringArm = Player_C->GetSpringArm();

        


    Player_C->ZoomIn();
    
}

void AWeaponBow::FireArrow()
{

    


    // 소켓 위치 및 회전 가져오기

    APlayerCharacter* Player_C = Cast<APlayerCharacter>(GetOwner());

    FVector SpawnLocation = GetActorLocation();

    // 조준 방향 계산: 예) 카메라 방향, 또는 컨트롤러 방향
    FVector AimDirection = Player_C->GetControlRotation().Vector(); // 또는 캐릭터 카메라 방향

    // 조준 방향을 회전으로 변환
    FRotator SpawnRotation = AimDirection.Rotation();

    // 스폰 파라미터
    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    SpawnParams.Owner = Player_C;

    // 화살 액터 스폰
    AProjectile_Arrow* Arrow = GetWorld()->SpawnActor<AProjectile_Arrow>(
        ArrowClass,
        SpawnLocation,
        SpawnRotation,
        SpawnParams
    );
    /*if (Player_C->GetIsFire())
    {
        Arrow->SetData(TEXT("Player_FireArrow"), TEXT("ToMonster"));
    }
    else {
        Arrow->SetData(TEXT("Player_Arrow"), TEXT("ToMonster"));
    }*/

    Arrow->SetData(TEXT("Player_Arrow"), TEXT("ToMonster"));

    Player_C->SetArrowFire(false);


    Player_C->SetArrowVisibility(false);

}
