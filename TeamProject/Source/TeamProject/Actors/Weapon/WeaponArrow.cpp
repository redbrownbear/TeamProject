// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponArrow.h"

AWeaponArrow::AWeaponArrow()
{

    DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
    RootComponent = DefaultSceneRoot;


    ConstructorHelpers::FObjectFinder<USkeletalMesh> Asset(TEXT("/Script/Engine.SkeletalMesh'/Game/Resources/Player/Arrow_Normal/ArrowNormal_A.ArrowNormal_A'"));

    SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
    SkeletalMeshComponent->SetupAttachment(RootComponent);
    SkeletalMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    SkeletalMeshComponent->SetSkeletalMesh(Asset.Object);
}
