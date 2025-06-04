// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Character/WeaponManagerComponent.h"
#include "Actors/Character/PlayerCharacter.h"
#include "Actors/Weapon/WeaponShield.h"
#include "Components/Character/PlayerMovementComponent.h"
#include "Animation/AnimInstance/PlayerAnimInstance.h"

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
	{
		Glider = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Glider"));
		ConstructorHelpers::FObjectFinder<USkeletalMesh> Asset{
			TEXT("/Script/Engine.SkeletalMesh'/Game/Resources/Player/Armor/Animation/Glide/Item_Parastole2_Vagrant.Item_Parastole2_Vagrant'")
		};

		if (Asset.Object)
		{
			Glider->SetSkeletalMesh(Asset.Object);
		}
	}
	if (Mesh)
	{
		Shield->SetupAttachment(Player_C->GetMesh(), TEXT("Shield_Socket"));
		Sword->SetupAttachment(Player_C->GetMesh(), TEXT("Sword_Socket"));
		Bow->SetupAttachment(Player_C->GetMesh(), TEXT("Bow_Socket"));
		Arrow->SetupAttachment(Player_C->GetMesh(), TEXT("Arrow_Normal"));
		Glider->SetupAttachment(Player_C->GetMesh(), TEXT("GliderSocket"));
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
	{
		ConstructorHelpers::FObjectFinder<UAnimMontage> Asset(TEXT("/Script/Engine.AnimMontage'/Game/Resources/Player/Armor/Animation/Glide/Equip_Float_On_Montage.Equip_Float_On_Montage'"));

		if (Asset.Object)
		{
			EquipGlider = Asset.Object;
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
			UnEquipGlider = Asset.Object;
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
	Glider->SetVisibility(false);
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
	
	if (CRT->GetCharacterMovement()->MovementMode == MOVE_None)
	{
		return;
	}
	

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
		else if (Equip_State == EEquip_State::Shield)
		{
			UAnimMontage* UnEquipMontage = Cast<AWeaponBase>(Shield->GetChildActor())->GetUnEquipMontage();
			
			UnEquipWeapons.Enqueue(EWeapon_Type::Shield);
			AnimInstance->Montage_Play(UnEquipMontage);
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
			AWeaponBase* WeaponBaseWeapon = Cast<AWeaponBase>(Shield->GetChildActor());
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

void UWeaponManagerComponent::LeftClickAction()
{
	APlayerCharacter* Player_C = Cast<APlayerCharacter>(GetOwner());
	if (Equip_State == EEquip_State::Sword)
	{
		if (Player_C->JumpCurrentCount == 1)
			return;


		AWeaponSword* SwordActor = Cast<AWeaponSword>(Sword->GetChildActor());

		if (!SwordActor)
		{

			UE_LOG(LogTemp, Warning, TEXT("SwordActor is not Valid"));
			return;

		}
		SwordActor->LeftClickAction();

	}
	
	else if (Equip_State == EEquip_State::Bow)
	{
		if (!bCanShot)
		{
			return;
		}
		AWeaponBow* BowActor = Cast<AWeaponBow>(Bow->GetChildActor());

		if (!BowActor)
		{

			UE_LOG(LogTemp, Warning, TEXT("BowActor is not Valid"));
			return;

		}
		BowActor->LeftClickAction();
	}

	else if (Equip_State == EEquip_State::Sword_Shield)
	{
		if (Player_C->JumpCurrentCount == 1)
			return;
		if (bRightClick)
		{
			if (!bCanShot)
			{
				return;
			}
			AWeaponShield* ShieldActor = Cast<AWeaponShield>(Shield->GetChildActor());
			if (!ShieldActor)
			{
				UE_LOG(LogTemp, Warning, TEXT("ShieldActor is not Valid"));
			}
			ShieldActor->LeftClickAction();
			return;
		
		}
		else
		{
			AWeaponSword* SwordActor = Cast<AWeaponSword>(Sword->GetChildActor());

			if (!SwordActor)
			{

				UE_LOG(LogTemp, Warning, TEXT("SwordActor is not Valid"));
				return;

			}
			SwordActor->LeftClickAction();
		}
	}
	else if(Equip_State == EEquip_State::Shield)
	{
		if (Player_C->JumpCurrentCount == 1)
			return;
		if (bRightClick)
		{
			if (!bCanShot)
			{
				return;
			}
			AWeaponShield* ShieldActor = Cast<AWeaponShield>(Shield->GetChildActor());
			if (!ShieldActor)
			{
				UE_LOG(LogTemp, Warning, TEXT("ShieldActor is not Valid"));
			}
			ShieldActor->LeftClickAction();
		}
	}

}

void UWeaponManagerComponent::RightClickAction()
{
	APlayerCharacter* Player_C = Cast<APlayerCharacter>(GetOwner());

	if ("NULL" == Player_C->GetCharacterMovement()->GetMovementName())
	{
		return;
	}

	if (Equip_State == EEquip_State::None || Equip_State == EEquip_State::Sword)
	{
		return;
	}

	else if(Equip_State == EEquip_State::Bow)
	{

		AWeaponBow* BowActor = Cast<AWeaponBow>(Bow->GetChildActor());

		if (!BowActor)
		{

			UE_LOG(LogTemp, Warning, TEXT("BowActor is not Valid"));
			return;

		}

		BowActor->RightClickAction();
	}

	else
	{

		AWeaponShield* ShieldActor = Cast<AWeaponShield>(Shield->GetChildActor());

		if (!ShieldActor)
		{
			UE_LOG(LogTemp, Warning, TEXT("Shield is not Valid"));
			return;
			//check(ShieldActor);
		}

		ShieldActor->RightClickAction();
	}
	bRightClick = true;
}

void UWeaponManagerComponent::RightClickEnd()
{


	APlayerCharacter* Player_C = Cast<APlayerCharacter>(GetOwner());

	UPlayerAnimInstance* AnimInst = Cast<UPlayerAnimInstance>(Player_C->GetMesh()->GetAnimInstance());

	if (Equip_State == EEquip_State::Bow)
	{
		if (AnimInst->bIsZoom)
		{

			

			AnimInst->bIsZoom = false;
			UPlayerMovementComponent* Movement = Cast<UPlayerMovementComponent>(Player_C->GetCharacterMovement());
			Movement->MaxWalkSpeed = PLAYER_MOVE_NML;
			Movement->SetMoveState(EMove_State::Run);
			Movement->bOrientRotationToMovement = true;

			Player_C->bUseControllerRotationYaw = false; // 컨트롤러 Yaw 방향을 따라 캐릭터 회전


			// 이동 방향으로 자동 회전 비활성화
			

			Player_C->ZoomOut();


			AWeaponBow* WBow = Cast<AWeaponBow>(Bow->GetChildActor());

			if (AnimInst->Montage_IsPlaying(WBow->GetChargingMTG()))
			{
				AnimInst->Montage_Stop(0.f);
			}
			Player_C->SetArrowVisibility(false);

			Player_C->SetNiagaraSystemAssetNone();
		}
	}
	else if (Equip_State == EEquip_State::Shield || Equip_State == EEquip_State::Sword_Shield)
	{
		if (AnimInst->bIsWaitShield)
		{
			

			AnimInst->bIsWaitShield = false;


			

			Player_C->bUseControllerRotationYaw = false; // 컨트롤러 Yaw 방향을 따라 캐릭터 회전
			UPlayerMovementComponent* Movement = Cast<UPlayerMovementComponent>(Player_C->GetCharacterMovement());
			Movement->MaxWalkSpeed = PLAYER_MOVE_NML;
			Movement->SetMoveState(EMove_State::Run);
			Movement->bOrientRotationToMovement = true;

			// 이동 방향으로 자동 회전 비활성화
			

			Player_C->ZoomOut();

			AWeaponShield* WShield = Cast<AWeaponShield>(Shield->GetChildActor());

			if (AnimInst->Montage_IsPlaying(WShield->GetWaitMTG()))
			{
				AnimInst->Montage_Stop(0.f);
			}
		}
	}
	bRightClick = false;
	bCanShot = false;
}
