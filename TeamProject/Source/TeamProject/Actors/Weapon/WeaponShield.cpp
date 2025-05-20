// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponShield.h"

AWeaponShield::AWeaponShield()
{
    {

        PrimaryActorTick.bCanEverTick = true;

        DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
        RootComponent = DefaultSceneRoot;


        ConstructorHelpers::FObjectFinder<USkeletalMesh> Asset(TEXT("/Script/Engine.SkeletalMesh'/Game/Resources/Player/Shield/Weapon_Shield_001.Weapon_Shield_001'"));

        SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
        SkeletalMeshComponent->SetupAttachment(RootComponent);
        SkeletalMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        SkeletalMeshComponent->SetSkeletalMesh(Asset.Object);

    }
}
