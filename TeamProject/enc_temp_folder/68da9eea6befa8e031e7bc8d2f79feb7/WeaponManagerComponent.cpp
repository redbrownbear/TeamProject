// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Character/WeaponManagerComponent.h"
#include "Actors/Character/PlayerCharacter.h"
#include "Actors/Weapon/WeaponShield.h"

// Sets default values for this component's properties
UWeaponManagerComponent::UWeaponManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...

	APlayerCharacter* Player_C = Cast<APlayerCharacter>(GetOwner());

	if (!Player_C)
	{
		return;
	}
	USkeletalMeshComponent* Mesh = Player_C->GetMesh();

	Shield = CreateDefaultSubobject<UWeaponChildActorComponent>(TEXT("Shield"));
	
	Shield->SetChildActorClass(AWeaponShield::StaticClass());

	Sword = CreateDefaultSubobject<UWeaponChildActorComponent>(TEXT("Sword"));
	
	Sword->SetChildActorClass(AWeaponSword::StaticClass());

	Bow = CreateDefaultSubobject<UWeaponChildActorComponent>(TEXT("Bow"));
	
	Bow->SetChildActorClass(AWeaponBow::StaticClass());

	Arrow = CreateDefaultSubobject<UWeaponChildActorComponent>(TEXT("ArrowNormal"));
	
	Arrow->SetChildActorClass(AWeaponArrow::StaticClass());

	if (Mesh)
	{
		Shield->SetupAttachment(Player_C->GetMesh(), TEXT("Shield_Socket"));
		Sword->SetupAttachment(Player_C->GetMesh(), TEXT("Sword_Socket"));
		Bow->SetupAttachment(Player_C->GetMesh(), TEXT("Bow_Socket"));
		Arrow->SetupAttachment(Player_C->GetMesh(), TEXT("Arrow_Normal"));
	}

}


// Called when the game starts
void UWeaponManagerComponent::BeginPlay()
{
	Super::BeginPlay();
	Sword->GetChildActor()->SetOwner(GetOwner());
	Shield->GetChildActor()->SetOwner(GetOwner());
	Bow->GetChildActor()->SetOwner(GetOwner());

	// ...
	
}


// Called every frame
void UWeaponManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UWeaponManagerComponent::TryEquipWeapon()
{
	ACharacter* CRT = Cast<ACharacter>(GetOwner());
	USkeletalMeshComponent* Mesh = CRT->GetMesh();
	if (NextWeapon == EWeapon_Type::Sword)
	{
		if (Equip_State == EEquip_State::Sword || Equip_State == EEquip_State::Sword_Shield)
		{
			
			AWeaponBase* WeaponBaseWeapon = Cast<AWeaponBase>(Sword->GetChildActor());
			
			UAnimMontage* UnEquipMontage = WeaponBaseWeapon->GetUnEquipMontage();
			
			UnEquipWeapons.Enqueue(EWeapon_Type::Sword);

			Mesh->GetAnimInstance()->Montage_Play(UnEquipMontage);
		}
		else if (Equip_State == EEquip_State::Bow)
		{
			
			

			UAnimInstance* AnimInstance = Mesh->GetAnimInstance();
			UAnimMontage* UnEquipMontage = Cast<AWeaponBase>(Bow->GetChildActor())->GetUnEquipMontage();
			AnimInstance->Montage_Play(UnEquipMontage);
			FOnMontageEnded MontageEndedDelegate = FOnMontageEnded::CreateUObject<UWeaponManagerComponent>(
				this,
				&UWeaponManagerComponent::EquipWeapon
			);
			AnimInstance->Montage_SetEndDelegate(MontageEndedDelegate, UnEquipMontage);

		}
		else
		{
			AWeaponBase* WeaponBaseWeapon = Cast<AWeaponBase>(Sword->GetChildActor());
			UAnimMontage* EquipMontage = WeaponBaseWeapon->GetEquipMontage();

			Mesh->GetAnimInstance()->Montage_Play(EquipMontage);
		}
	}

}

void UWeaponManagerComponent::EquipWeapon(UAnimMontage* Montage, bool bInterrupted)
{
	ACharacter* CRT = Cast<ACharacter>(GetOwner());
	USkeletalMeshComponent* Mesh = CRT->GetMesh();
	if (NextWeapon == EWeapon_Type::Bow)
	{
		AWeaponBase* WeaponBaseWeapon = Cast<AWeaponBase>(Bow->GetChildActor());
		UAnimMontage* EquipMontage = WeaponBaseWeapon->GetEquipMontage();

		Mesh->GetAnimInstance()->Montage_Play(EquipMontage);
	}
	else if (NextWeapon == EWeapon_Type::Sword)
	{
		AWeaponBase* WeaponBaseWeapon = Cast<AWeaponBase>(Sword->GetChildActor());
		UAnimMontage* EquipMontage = WeaponBaseWeapon->GetEquipMontage();

		Mesh->GetAnimInstance()->Montage_Play(EquipMontage);
	}
	else if (NextWeapon == EWeapon_Type::Shield)
	{
		AWeaponBase* WeaponBaseWeapon = Cast<AWeaponBase>(Shield->GetChildActor());
		UAnimMontage* EquipMontage = WeaponBaseWeapon->GetEquipMontage();

		Mesh->GetAnimInstance()->Montage_Play(EquipMontage);
	}


}