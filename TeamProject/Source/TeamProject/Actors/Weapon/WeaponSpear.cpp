// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Weapon/WeaponSpear.h"

AWeaponSpear::AWeaponSpear()
{
	{
		ConstructorHelpers::FObjectFinder<USpearAttackDataAsset> Asset(TEXT("/Script/TeamProject.SpearAttackDataAsset'/Game/Resources/Player/Assets/SpearAttackDataAsset.SpearAttackDataAsset'"));

		if (Asset.Object)
		{
			DataAsset = Asset.Object;
		}
	}

	{
		ConstructorHelpers::FObjectFinder<UStaticMesh> Asset(TEXT("/Script/Engine.StaticMesh'/Game/Resources/Weapon/SpearStaticMesh/Spear_001.Spear_001'"));

		if (Asset.Object)
		{
			StaticMeshComponent->SetStaticMesh(Asset.Object);
		}
	}
}


