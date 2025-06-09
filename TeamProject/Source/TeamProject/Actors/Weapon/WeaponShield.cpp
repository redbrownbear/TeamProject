// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponShield.h"
#include "Actors/Character/PlayerCharacter.h"
#include "Components/Character/PlayerMovementComponent.h"
#include "SubSystem/PlayerManager.h"
#include "Animation/AnimInstance/PlayerAnimInstance.h"

AWeaponShield::AWeaponShield()
{
    {

        PrimaryActorTick.bCanEverTick = true;

        DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
        RootComponent = DefaultSceneRoot;


        ConstructorHelpers::FObjectFinder<UStaticMesh> Asset(TEXT("/Script/Engine.StaticMesh'/Game/Resources/Weapon/ShieldStaticMesh/Shield_001.Shield_001'"));

        StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
        StaticMeshComponent->SetupAttachment(RootComponent);
        StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        StaticMeshComponent->SetStaticMesh(Asset.Object);

    }

    {
        ConstructorHelpers::FObjectFinder<UAnimMontage> Asset(TEXT("/Script/Engine.AnimMontage'/Game/Resources/Player/Shield/Animation/Equip_Shield_On_Montage.Equip_Shield_On_Montage'"));

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
        ConstructorHelpers::FObjectFinder<UAnimMontage> Asset(TEXT("/Script/Engine.AnimMontage'/Game/Resources/Player/Shield/Animation/Equip_Shield_Off_Natural_Montage.Equip_Shield_Off_Natural_Montage'"));

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
        ConstructorHelpers::FObjectFinder<UAnimMontage> Asset(TEXT("/Script/Engine.AnimMontage'/Game/Resources/Player/Armor/Animation/Shield_Just/Shield_Guard_Wait_Montage.Shield_Guard_Wait_Montage'"));

        if (Asset.Object)
        {
            Wait_MTG = Asset.Object;
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("No Shield_Wait_Montage"));
            
        }


    }



    {

        ConstructorHelpers::FObjectFinder<UAnimMontage> Asset(TEXT("/Script/Engine.AnimMontage'/Game/Resources/Player/Armor/Animation/Shield_Just/Shield_Guard_Just_Montage.Shield_Guard_Just_Montage'"));

        if (Asset.Object)
        {
            Just_MTG = Asset.Object;
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("No Shield_Wait_Montage"));
            
        }

    }

}

void AWeaponShield::SetGuard()
{
    UWeaponManagerComponent* WeaponManagerComponent = Cast<APlayerCharacter>(GetOwner())->GetWeaponManagerComponent();
    WeaponManagerComponent->SetIsGuard(true);
}

void AWeaponShield::LeftClickAction()
{

    
    {
        APlayerCharacter* Player_C = Cast<APlayerCharacter>(GetOwner());

        Player_C->GetWeaponManagerComponent()->SetCanShot(false);

        UPlayerAnimInstance* AnimInst = Cast<UPlayerAnimInstance>(Player_C->GetMesh()->GetAnimInstance());

        

        if (AnimInst->Montage_IsPlaying(Wait_MTG))
        {

            AnimInst->Montage_Stop(0.f);
            AnimInst->Montage_Play(Just_MTG);
            AnimInst->bIsWaitShield = false;

        }
        Player_C->GetCharacterMovement()->SetMovementMode(MOVE_None);

        Player_C->bUseControllerRotationYaw = false; // 컨트롤러 Yaw 방향을 따라 캐릭터 회전

        UPlayerMovementComponent* Movement = Cast<UPlayerMovementComponent>(Player_C->GetCharacterMovement());

        Movement->bOrientRotationToMovement = true;

        Movement->MaxWalkSpeed = PLAYER_MOVE_NML;

        SetGuard();

        Movement->SetMoveState(EMove_State::Run);

        
    }




}

void AWeaponShield::RightClickAction()
{

    APlayerCharacter* Player_C = Cast<APlayerCharacter>(GetOwner());

    UPlayerAnimInstance* AnimInst = Cast<UPlayerAnimInstance>(Player_C->GetMesh()->GetAnimInstance());

    if (AnimInst->Montage_IsPlaying(EquipMontage))
    {
        return;
    }

    if (AnimInst->bIsWaitShield)
    {
        return;
    }

    
    

    AnimInst->bIsWaitShield = true;

    AnimInst->Montage_Play(Wait_MTG);
    UPlayerMovementComponent* Movement = Cast<UPlayerMovementComponent>(Player_C->GetCharacterMovement());

    Movement->MaxWalkSpeed = PLAYER_MOVE_BOW_ZOOM;

    Movement->SetMoveState(EMove_State::Zoom);

    UPlayerManager* PlayerManager = GetGameInstance()->GetSubsystem<UPlayerManager>();
    PlayerManager->SetStaminaUSe(false);

    Movement->bOrientRotationToMovement = false;
    
    
    Player_C->bUseControllerRotationYaw = true; // 컨트롤러 Yaw 방향을 따라 캐릭터 회전

    // 이동 방향으로 자동 회전 비활성화
    

    USpringArmComponent* C_SpringArm = Player_C->GetSpringArm();



}
