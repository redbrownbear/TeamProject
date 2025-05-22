// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Character/PlayerCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "Actors/Weapon/WeaponBase.h"
#include "Actors/Weapon/WeaponSword.h"
#include "Actors/Weapon/WeaponShield.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "Engine/SkeletalMesh.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;



	FVector Scale = FVector(45.f, 45.f, 45.f);
	FVector Locate = FVector(0.f, 0.f, -43.f);
	{
		//SwordComponent = CreateDefaultSubobject<UWeaponComponent>(TEXT("SwordComponent"));
	}

	//SpringArm, Camera 생성 및 초기화
	{
		SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
		SpringArm->SetupAttachment(RootComponent);
		SpringArm->TargetArmLength = 400.f;
		

		{
			SpringArm->SetupAttachment(RootComponent);
			SpringArm->ProbeSize = 5.0;
			SpringArm->bUsePawnControlRotation = true;
			SpringArm->bInheritRoll = false;
		}
		Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
		Camera->SetupAttachment(SpringArm);
	}

	UCharacterMovementComponent* Movement = GetCharacterMovement();
	Movement->RotationRate = FRotator(0.0, 720.0, 0.0);
	Movement->bOrientRotationToMovement = true;
	


	{
		ConstructorHelpers::FObjectFinder<USkeletalMesh> Smesh{ (TEXT("/Script/Engine.SkeletalMesh'/Game/Resources/Player/Armor/ArmorMix/ArmorMix.ArmorMix'")) };
		USkeletalMeshComponent* mMesh = GetMesh();
		mMesh->SetupAttachment(RootComponent);
		mMesh->SetRelativeLocation(Locate);
		mMesh->SetSkeletalMeshAsset(Smesh.Object);
		ConstructorHelpers::FClassFinder<UAnimInstance> Anim{ (TEXT("/Script/Engine.AnimBlueprint'/Game/Resources/Player/Armor/BP_PlayerAnimInstance.BP_PlayerAnimInstance_C'")) };
		mMesh->SetAnimInstanceClass(Anim.Class);
		mMesh->SetRelativeScale3D(Scale);
	}
	{
		UCapsuleComponent* CC = GetCapsuleComponent();
		CC->SetCapsuleRadius(20.f);
		CC->SetCapsuleHalfHeight(41.f);

	}

	{
		StatusComponent = CreateDefaultSubobject<UPlayerStatusComponent>(TEXT("PlayerStatus"));
	}
	
	WeaponManagerComponent = CreateDefaultSubobject<UWeaponManagerComponent>(TEXT("WeaponManager"));
	bUseControllerRotationYaw = false;
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	//Arrow->GetChildActor()->SetOwner(this);
	SpringArm->ProbeChannel = ECC_GameTraceChannel1;

	GetMesh()->SetCollisionProfileName(TEXT("Player"));
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void APlayerCharacter::OnConstruction(const FTransform& Transform)
{
	
	

}

void APlayerCharacter::LeftClickAction()
{

}

void APlayerCharacter::RightClickAction()
{

}




