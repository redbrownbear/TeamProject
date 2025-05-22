// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Weapon/WeaponBow.h"
#include "Actors/Character/PlayerCharacter.h"

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
    Player_C->GetMesh()->GetAnimInstance()->Montage_Resume(Attack_MTG);

    Player_C->GetWeaponManagerComponent()->SetRightClick(false);

    Player_C->GetCharacterMovement()->MaxWalkSpeed = 600.f;
}

void AWeaponBow::RightClickAction()
{

    APlayerCharacter* Player_C = Cast<APlayerCharacter>(GetOwner());
    Player_C->GetMesh()->GetAnimInstance()->Montage_Play(Attack_MTG);
    
}
