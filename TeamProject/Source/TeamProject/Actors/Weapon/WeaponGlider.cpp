// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Weapon/WeaponGlider.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
AWeaponGlider::AWeaponGlider()
{
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	RootComponent = StaticMeshComponent;
	{
		ConstructorHelpers::FObjectFinder<UStaticMesh> Asset(TEXT("/Script/Engine.StaticMesh'/Game/Resources/Weapon/GliderStaticMesh/Glider.Glider'"));

		if (Asset.Object)
		{
			StaticMeshComponent->SetStaticMesh(Asset.Object);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("No Anim_Montage"));
		}
	}
	{
		ConstructorHelpers::FObjectFinder<UAnimMontage> Asset(TEXT("/Script/Engine.AnimMontage'/Game/Resources/Player/Armor/Animation/Glide/Equip_Float_On_Montage.Equip_Float_On_Montage'"));

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
		ConstructorHelpers::FObjectFinder<UAnimMontage> Asset(TEXT("/Script/Engine.AnimSequence'/Game/Resources/Player/Armor/Animation/Glide/Equip_Float_Off.Equip_Float_Off'"));

		if (Asset.Object)
		{
			UnEquipMontage = Asset.Object;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("No Anim_Montage"));
		}
	}
	

	LNiagaraSystemComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("LNiagaraSystemComponent"));
	RNiagaraSystemComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("RNiagaraSystemComponent"));
	
	{
		ConstructorHelpers::FObjectFinder<UNiagaraSystem> Asset(TEXT("/Script/Niagara.NiagaraSystem'/Game/Vefects/Custom/GliderSystem.GliderSystem'"));
		if (Asset.Object)
		{
			NiagaraFX = Asset.Object;
		}
	}
	if (LNiagaraSystemComponent && NiagaraFX)
	{
		LNiagaraSystemComponent->SetAsset(NiagaraFX);
	}
	if (RNiagaraSystemComponent && NiagaraFX)
	{
		RNiagaraSystemComponent->SetAsset(NiagaraFX);
	}
	FRotator MeshRotation = StaticMeshComponent->GetRelativeRotation();
	MeshRotation.Yaw += 180;
	LNiagaraSystemComponent->SetRelativeRotation(MeshRotation);
	RNiagaraSystemComponent->SetRelativeRotation(MeshRotation);
	
	

}

void AWeaponGlider::BeginPlay()
{
	Super::BeginPlay();
	LNiagaraSystemComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("LCASCADE"));
	RNiagaraSystemComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("RCASCADE"));
	SetVisibleGldier(false);
}

void AWeaponGlider::SetVisibleGldier(bool bFlag)
{
	StaticMeshComponent->SetVisibility(bFlag);
	bFlag ? StartTrailEffect() : EndTrailEffect();
}

void AWeaponGlider::StartTrailEffect()
{

	
	LNiagaraSystemComponent->Activate();
	RNiagaraSystemComponent->Activate();
	
}

void AWeaponGlider::EndTrailEffect()
{

	RNiagaraSystemComponent->Deactivate();
	LNiagaraSystemComponent->Deactivate();
}
