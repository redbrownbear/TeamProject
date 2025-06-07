// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponArrow.h"
#include "Actors/Character/PlayerCharacter.h"

AWeaponArrow::AWeaponArrow()
{

    DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
    RootComponent = DefaultSceneRoot;


    ConstructorHelpers::FObjectFinder<UStaticMesh> Asset(TEXT("/Script/Engine.SkeletalMesh'/Game/Resources/Player/Arrow_Normal/ArrowNormal_A.ArrowNormal_A'"));

    StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
    StaticMeshComponent->SetupAttachment(RootComponent);
    StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    StaticMeshComponent->SetStaticMesh(Asset.Object);
}


