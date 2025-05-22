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

	{
		ConstructorHelpers::FObjectFinder<UAnimMontage> Asset(TEXT("/Script/Engine.AnimMontage'/Game/Resources/Player/Sword_Shield/Animation/Sword_Shield_Off.Sword_Shield_Off'"));

		if (Asset.Object)
		{
			UnEquip_Sword_Shield = Asset.Object;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("No Anim_Montage"));
		}
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

	UAnimInstance* AnimInstance = Mesh->GetAnimInstance();


	UAnimMontage* PlayingMontage = AnimInstance->GetCurrentActiveMontage();
	if (PlayingMontage)
		return;
	

	if (NextWeapon == EWeapon_Type::Sword)
	{
		if (Equip_State == EEquip_State::Sword || Equip_State == EEquip_State::Sword_Shield)
		{
			
			AWeaponBase* WeaponBaseWeapon = Cast<AWeaponBase>(Sword->GetChildActor());
			
			UAnimMontage* UnEquipMontage = WeaponBaseWeapon->GetUnEquipMontage();
			
			UnEquipWeapons.Enqueue(EWeapon_Type::Sword);

			AnimInstance->Montage_Play(UnEquipMontage);
		}
		else if (Equip_State == EEquip_State::Bow)
		{
			
			UAnimMontage* UnEquipMontage = Cast<AWeaponBase>(Bow->GetChildActor())->GetUnEquipMontage();
			AnimInstance->Montage_Play(UnEquipMontage);

			UnEquipWeapons.Enqueue(EWeapon_Type::Bow);

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

			AnimInstance->Montage_Play(EquipMontage);
		}
	}
	else if (NextWeapon == EWeapon_Type::Bow)
	{
		if (Equip_State == EEquip_State::Bow)
		{

			AWeaponBase* WeaponBaseWeapon = Cast<AWeaponBase>(Bow->GetChildActor());

			UAnimMontage* UnEquipMontage = WeaponBaseWeapon->GetUnEquipMontage();

			UnEquipWeapons.Enqueue(EWeapon_Type::Bow);

			AnimInstance->Montage_Play(UnEquipMontage);
		}
		else if (Equip_State == EEquip_State::Sword)
		{

			UAnimMontage* UnEquipMontage = Cast<AWeaponBase>(Sword->GetChildActor())->GetUnEquipMontage();

			UnEquipWeapons.Enqueue(EWeapon_Type::Sword);

			AnimInstance->Montage_Play(UnEquipMontage);
			FOnMontageEnded MontageEndedDelegate = FOnMontageEnded::CreateUObject<UWeaponManagerComponent>(
				this,
				&UWeaponManagerComponent::EquipWeapon
			);
			AnimInstance->Montage_SetEndDelegate(MontageEndedDelegate, UnEquipMontage);

		}
		else if (Equip_State == EEquip_State::Sword_Shield)
		{
		

			UnEquipWeapons.Enqueue(EWeapon_Type::Sword);
			UnEquipWeapons.Enqueue(EWeapon_Type::Shield);
			AnimInstance->Montage_Play(UnEquip_Sword_Shield);
			FOnMontageEnded MontageEndedDelegate = FOnMontageEnded::CreateUObject<UWeaponManagerComponent>(
				this,
				&UWeaponManagerComponent::EquipWeapon
			);
			AnimInstance->Montage_SetEndDelegate(MontageEndedDelegate, UnEquip_Sword_Shield);


		}
		else
		{
			AWeaponBase* WeaponBaseWeapon = Cast<AWeaponBase>(Sword->GetChildActor());
			UAnimMontage* EquipMontage = WeaponBaseWeapon->GetEquipMontage();

			AnimInstance->Montage_Play(EquipMontage);
		}
	}
	else if (NextWeapon == EWeapon_Type::Shield)
	{
		if (Equip_State == EEquip_State::Shield || Equip_State == EEquip_State::Sword_Shield)
		{

			AWeaponBase* WeaponBaseWeapon = Cast<AWeaponBase>(Shield->GetChildActor());

			UAnimMontage* UnEquipMontage = WeaponBaseWeapon->GetUnEquipMontage();

			UnEquipWeapons.Enqueue(EWeapon_Type::Shield);

			AnimInstance->Montage_Play(UnEquipMontage);
		}
		else if (Equip_State == EEquip_State::Bow)
		{



		
			UAnimMontage* UnEquipMontage = Cast<AWeaponBase>(Bow->GetChildActor())->GetUnEquipMontage();
			AnimInstance->Montage_Play(UnEquipMontage);

			UnEquipWeapons.Enqueue(EWeapon_Type::Bow);

			FOnMontageEnded MontageEndedDelegate = FOnMontageEnded::CreateUObject<UWeaponManagerComponent>(
				this,
				&UWeaponManagerComponent::EquipWeapon
			);
			AnimInstance->Montage_SetEndDelegate(MontageEndedDelegate, UnEquipMontage);

		}
		else
		{
			AWeaponBase* WeaponBaseWeapon = Cast<AWeaponBase>(Bow->GetChildActor());
			UAnimMontage* EquipMontage = WeaponBaseWeapon->GetEquipMontage();

			AnimInstance->Montage_Play(EquipMontage);
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