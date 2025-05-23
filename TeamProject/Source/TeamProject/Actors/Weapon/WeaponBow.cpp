// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Weapon/WeaponBow.h"
#include "Actors/Character/PlayerCharacter.h"
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
}

void AWeaponBow::LeftClickAction()
{
    APlayerCharacter* Player_C = Cast<APlayerCharacter>(GetOwner());

    UPlayerAnimInstance* AnimInst = Cast<UPlayerAnimInstance>(Player_C->GetMesh()->GetAnimInstance());



    Player_C->GetWeaponManagerComponent()->SetCanShot(false);

    Player_C->GetWeaponManagerComponent()->SetRightClick(false);

    AnimInst->bIsZoom = false;
    
    AnimInst->Montage_Resume(Attack_MTG);

    Player_C->GetCharacterMovement()->MaxWalkSpeed = 600.f;


    Player_C->bUseControllerRotationYaw = false; // 컨트롤러 Yaw 방향을 따라 캐릭터 회전


    // 이동 방향으로 자동 회전 비활성화
    Player_C->GetCharacterMovement()->bOrientRotationToMovement = true;

    USpringArmComponent* C_SpringArm = Player_C->GetSpringArm();
  

    

    Player_C->ZoomOut();
}

void AWeaponBow::RightClickAction(bool _bool)
{
    
    APlayerCharacter* Player_C = Cast<APlayerCharacter>(GetOwner());


    UPlayerAnimInstance* AnimInst = Cast<UPlayerAnimInstance>(Player_C->GetMesh()->GetAnimInstance());

    AnimInst->bIsZoom = _bool;
    
    if (_bool)
    {

        AnimInst->Montage_Play(Attack_MTG);
        UCharacterMovementComponent* C_Movement = Player_C->GetCharacterMovement();

        C_Movement->MaxWalkSpeed = 300;

        Player_C->bUseControllerRotationYaw = true; // 컨트롤러 Yaw 방향을 따라 캐릭터 회전

        // 이동 방향으로 자동 회전 비활성화
        C_Movement->bOrientRotationToMovement = false;

        USpringArmComponent* C_SpringArm = Player_C->GetSpringArm();
        
        


        Player_C->ZoomIn();
    }

    else
    {
        AnimInst->Montage_Stop(0.f);
        
        Player_C->GetCharacterMovement()->MaxWalkSpeed = 600;

        Player_C->bUseControllerRotationYaw = false; // 컨트롤러 Yaw 방향을 따라 캐릭터 회전

        
        // 이동 방향으로 자동 회전 비활성화
        Player_C->GetCharacterMovement()->bOrientRotationToMovement = true;

        Player_C->ZoomOut();


    }
    
}
