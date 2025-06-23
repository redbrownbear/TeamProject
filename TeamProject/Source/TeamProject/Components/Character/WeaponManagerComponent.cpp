// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Character/WeaponManagerComponent.h"
#include "Actors/Character/PlayerCharacter.h"
#include "Actors/Weapon/WeaponShield.h"
#include "Components/Character/PlayerMovementComponent.h"
#include "Animation/AnimInstance/PlayerAnimInstance.h"
#include "UI/HUD/MainHUD.h"
#include "Actors/Weapon/WeaponSpear.h"
#include "SubSystem/PlayerManager.h"
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

	Weapon = CreateDefaultSubobject<UWeaponChildActorComponent>(TEXT("Weapon"));
	
	Weapon->SetChildActorClass(AWeaponSword::StaticClass());

	Bow = CreateDefaultSubobject<UWeaponChildActorComponent>(TEXT("Bow"));
	
	Bow->SetChildActorClass(AWeaponBow::StaticClass());

	Arrow = CreateDefaultSubobject<UWeaponChildActorComponent>(TEXT("ArrowNormal"));
	
	Arrow->SetChildActorClass(AWeaponArrow::StaticClass());
	
	Glider = CreateDefaultSubobject<UWeaponChildActorComponent>(TEXT("Glider"));
	

	Glider->SetChildActorClass(AWeaponGlider::StaticClass());
	if (Mesh)
	{
		Shield->SetupAttachment(Player_C->GetMesh(), TEXT("Shield_Socket"));
		Weapon->SetupAttachment(Player_C->GetMesh(), TEXT("Sword_Socket"));
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


}


// Called when the game starts
void UWeaponManagerComponent::BeginPlay()
{
	Super::BeginPlay();
	Weapon->GetChildActor()->SetOwner(GetOwner());
	Shield->GetChildActor()->SetOwner(GetOwner());
	Bow->GetChildActor()->SetOwner(GetOwner());
	Glider->GetChildActor()->SetOwner(GetOwner());
	// ...
	UPlayerManager* PlayerManager = GetOwner()->GetGameInstance()->GetSubsystem<UPlayerManager>();
	TArray<FItemData> EquipData = PlayerManager->GetAllEquipData();

	for (auto& Data : EquipData)
	{
		switch (Data.eItemCategory)
		{
		case EItemCategory::IT_Weapon:
		
			switch (Data.eWeaponKind)
			{
			case EWeaponKind::None:
				break;
			case EWeaponKind::SWORD:
				SetWeaponStaticMesh(Data.StaticMesh, EWeaponKind::SWORD);
				break;
			case EWeaponKind::SPEAR:
				SetWeaponStaticMesh(Data.StaticMesh, EWeaponKind::SPEAR);
				break;
			case EWeaponKind::LSWORD:
				break;
			case EWeaponKind::BOW:
				SetBowStaticMesh(Data.StaticMesh);
				break;
			case EWeaponKind::END:
				break;
			default:
				break;
			}
			break;
		case EItemCategory::IT_Shield:
			SetShieldStaticMesh(Data.StaticMesh);
		}
	}
	NextWeapon = EWeapon_Type::None;
}


// Called every frame
void UWeaponManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UWeaponManagerComponent::WhatWeaponKind(EWeapon_Type InType)
{
	if (InType == EWeapon_Type::Sword)
	{
		InType = Cast<AWeaponBase>(Weapon->GetChildActor())->GetWeaponType();
	}
	SetNextWeaponType(InType);

	TryEquipWeapon();
}

void UWeaponManagerComponent::SetEquipState(EEquip_State _State)
{
	Equip_State = _State;
	OnEquipStateUpdate.Broadcast(_State);
}

void UWeaponManagerComponent::SetBowStaticMesh(UStaticMesh* InMesh)
{
	AWeaponBase* WeaponBase = Cast<AWeaponBase>(Bow->GetChildActor());
	WeaponBase->SetStaticMesh(InMesh);
}

void UWeaponManagerComponent::SetWeaponStaticMesh(UStaticMesh* InMesh, EWeaponKind WeaponKind)
{
	if (WeaponKind == EWeaponKind::SWORD)
	{
		GetWeapon()->SetChildActorClass(AWeaponSword::StaticClass());
		if (Equip_State == EEquip_State::Spear)
		{
			SetEquipState(EEquip_State::Sword);
		}
	}
	else if (WeaponKind == EWeaponKind::SPEAR)
	{
		GetWeapon()->SetChildActorClass(AWeaponSpear::StaticClass());

		if (Equip_State == EEquip_State::Sword || Equip_State == EEquip_State::Sword_Shield || Equip_State == EEquip_State::Shield)
		{

			SetEquipState(EEquip_State::Spear);

			USkeletalMeshComponent* CharacterRoot = Cast<ACharacter>(GetOwner())->GetMesh();
			Shield->AttachToComponent(CharacterRoot, FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("Shield_Socket"));
		}

	}
	AWeaponBase* WeaponBase = Cast<AWeaponBase>(Weapon->GetChildActor());
	WeaponBase->SetStaticMesh(InMesh);
}

void UWeaponManagerComponent::SetShieldStaticMesh(UStaticMesh* InMesh)
{
	AWeaponBase* WeaponBase = Cast<AWeaponBase>(Shield->GetChildActor());
	WeaponBase->SetStaticMesh(InMesh);
}

void UWeaponManagerComponent::SetCanAttack()
{
	if (AWeaponSword* Sword = Cast<AWeaponSword>(Weapon->GetChildActor()))
	{
		Sword->SetCanAttack();
	}
	else if (AWeaponSpear* Spear = Cast<AWeaponSpear>(Weapon->GetChildActor()))
	{

	}
}

bool UWeaponManagerComponent::GetIsHoldingShield()
{
	if (GetEquipState() == EEquip_State::Sword_Shield || GetEquipState() == EEquip_State::Shield)
	{
		return bCanShot;
	}
	return false;
}

void UWeaponManagerComponent::TryEquipWeapon()
{
	ACharacter* CRT = Cast<ACharacter>(GetOwner());
	USkeletalMeshComponent* Mesh = CRT->GetMesh();

	UAnimInstance* AnimInstance = Mesh->GetAnimInstance();

	UnEquipWeapons.Empty();
	UAnimMontage* PlayingMontage = AnimInstance->GetCurrentActiveMontage();
	if (PlayingMontage)
		return;
	
	if (CRT->GetCharacterMovement()->MovementMode == MOVE_None)
	{
		return;
	}

	SetWeaponSwapState(EWeapon_Swap_State::Swaping);

	if (NextWeapon == EWeapon_Type::Sword)
	{
		if (Equip_State == EEquip_State::Sword || Equip_State == EEquip_State::Sword_Shield)
		{
			Weapon->AttachToComponent(CRT->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("Sword_Socket"));
			SetWeaponSwapState(EWeapon_Swap_State::None);

			Equip_State == EEquip_State::Sword ? SetEquipState(EEquip_State::None) : SetEquipState(EEquip_State::Shield);
			
		}
		else if (Equip_State == EEquip_State::Bow)
		{
			Bow->AttachToComponent(CRT->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("Bow_Socket"));
			
			AWeaponBase* WeaponBaseWeapon = Cast<AWeaponBase>(Weapon->GetChildActor());
			UAnimMontage* EquipMontage = WeaponBaseWeapon->GetEquipMontage();

			AnimInstance->Montage_Play(EquipMontage);
		}
		else
		{
			AWeaponBase* WeaponBaseWeapon = Cast<AWeaponBase>(Weapon->GetChildActor());
			UAnimMontage* EquipMontage = WeaponBaseWeapon->GetEquipMontage();

			AnimInstance->Montage_Play(EquipMontage);
		}
	}
	else if (NextWeapon == EWeapon_Type::Bow)
	{
		if (Equip_State == EEquip_State::Bow)
		{
			Bow->AttachToComponent(CRT->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("Bow_Socket"));
			SetWeaponSwapState(EWeapon_Swap_State::None);
			SetEquipState(EEquip_State::None);
		}
		else if (Equip_State == EEquip_State::Sword)
		{
			Weapon->AttachToComponent(CRT->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("Sword_Socket"));

			AWeaponBase* WeaponBaseWeapon = Cast<AWeaponBase>(Bow->GetChildActor());
			UAnimMontage* EquipMontage = WeaponBaseWeapon->GetEquipMontage();

			AnimInstance->Montage_Play(EquipMontage);

		}
		else if (Equip_State == EEquip_State::Sword_Shield)
		{
			Weapon->AttachToComponent(CRT->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("Sword_Socket"));
			Shield->AttachToComponent(CRT->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("Shield_Socket"));
			
			AWeaponBase* WeaponBaseWeapon = Cast<AWeaponBase>(Bow->GetChildActor());
			UAnimMontage* EquipMontage = WeaponBaseWeapon->GetEquipMontage();

			AnimInstance->Montage_Play(EquipMontage);

		}
		else if (Equip_State == EEquip_State::Shield)
		{
			Shield->AttachToComponent(CRT->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("Shield_Socket"));

			AWeaponBase* WeaponBaseWeapon = Cast<AWeaponBase>(Bow->GetChildActor());
			UAnimMontage* EquipMontage = WeaponBaseWeapon->GetEquipMontage();

			AnimInstance->Montage_Play(EquipMontage);

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

			Shield->AttachToComponent(CRT->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("Shield_Socket"));
			SetWeaponSwapState(EWeapon_Swap_State::None);
			SetEquipState(EEquip_State::None);
		}
		else if (Equip_State == EEquip_State::Bow)
		{

			Bow->AttachToComponent(CRT->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("Bow_Socket"));

			AWeaponBase* WeaponBaseWeapon = Cast<AWeaponBase>(Shield->GetChildActor());
			UAnimMontage* EquipMontage = WeaponBaseWeapon->GetEquipMontage();

			AnimInstance->Montage_Play(EquipMontage);

		
		}
		else if (Equip_State == EEquip_State::Spear)
		{
			Weapon->AttachToComponent(CRT->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("Sword_Socket"));

			AWeaponBase* WeaponBaseWeapon = Cast<AWeaponBase>(Shield->GetChildActor());
			UAnimMontage* EquipMontage = WeaponBaseWeapon->GetEquipMontage();

			AnimInstance->Montage_Play(EquipMontage);
		}
		else
		{
			AWeaponBase* WeaponBaseWeapon = Cast<AWeaponBase>(Shield->GetChildActor());
			UAnimMontage* EquipMontage = WeaponBaseWeapon->GetEquipMontage();

			AnimInstance->Montage_Play(EquipMontage);
		}
	}

	else if (NextWeapon == EWeapon_Type::Spear)
	{
		if (Equip_State == EEquip_State::Bow)
		{
			Bow->AttachToComponent(CRT->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("Bow_Socket"));

			AWeaponBase* WeaponBaseWeapon = Cast<AWeaponBase>(Weapon->GetChildActor());
			UAnimMontage* EquipMontage = WeaponBaseWeapon->GetEquipMontage();

			AnimInstance->Montage_Play(EquipMontage);
		}
		else if(Equip_State == EEquip_State::Spear)
		{
			Weapon->AttachToComponent(CRT->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("Sword_Socket"));
			SetEquipState(EEquip_State::None);
			Cast<UPlayerAnimInstance>(AnimInstance)->Equip_State = EEquip_State::None;
			SetWeaponSwapState(EWeapon_Swap_State::None);
		}
		else if (Equip_State == EEquip_State::None)
		{
			AWeaponBase* WeaponBaseWeapon = Cast<AWeaponBase>(Weapon->GetChildActor());
			UAnimMontage* EquipMontage = WeaponBaseWeapon->GetEquipMontage();

			AnimInstance->Montage_Play(EquipMontage);
		}
	}
}



void UWeaponManagerComponent::LeftClickAction()
{
	APlayerCharacter* Player_C = Cast<APlayerCharacter>(GetOwner());
	if (Equip_State == EEquip_State::Sword)
	{
		if (Player_C->JumpCurrentCount == 1)
			return;


		AWeaponSword* SwordActor = Cast<AWeaponSword>(Weapon->GetChildActor());

		if (!SwordActor)
		{

			UE_LOG(LogTemp, Warning, TEXT("SwordActor is not Valid"));
			return;

		}
		SwordActor->LeftClickAction();

	}
	else if (Equip_State == EEquip_State::Spear)
	{
		if (Player_C->JumpCurrentCount == 1)
			return;


		AWeaponSpear* SpearActor = Cast<AWeaponSpear>(Weapon->GetChildActor());

		if (!SpearActor)
		{

			UE_LOG(LogTemp, Warning, TEXT("SwordActor is not Valid"));
			return;

		}
		SpearActor->LeftClickAction();

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
			AWeaponSword* SwordActor = Cast<AWeaponSword>(Weapon->GetChildActor());

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

	if (Equip_State == EEquip_State::None || Equip_State == EEquip_State::Sword || Equip_State == EEquip_State::Spear)
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

			Cast<AMainHUD>(GetWorld()->GetFirstPlayerController()->GetHUD())->ShowBowAimgUI(false, 0);

			AWeaponBow* WBow = Cast<AWeaponBow>(Bow->GetChildActor());

			if (AnimInst->Montage_IsPlaying(WBow->GetChargingMTG()))
			{
				AnimInst->Montage_Stop(0.f);
			}
			Player_C->SetArrowFire(false);

			Player_C->SetArrowVisibility(false);
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
