// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Character/PlayerCharacter.h"
#include "Components/SkeletalMeshComponent.h"

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
	FVector Locate = FVector(0.f, 0.f, -41.f);

	//SpringArm, Camera 생성 및 초기화
	{
		SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
		SpringArm->SetupAttachment(RootComponent);
		SpringArm->TargetArmLength = 500.f;

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
		ConstructorHelpers::FObjectFinder<USkeletalMesh> Smesh{ (TEXT("/Script/Engine.SkeletalMesh'/Game/Armor/Head/Armor_230_Head.Armor_230_Head'")) };
		Head = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HEAD"));
		Head->SetSkeletalMeshAsset(Smesh.Object);
		Head->SetupAttachment(RootComponent);
		Head->SetRelativeLocation(Locate);
		Head->SetRelativeScale3D(Scale);
		
	}

	{
		ConstructorHelpers::FObjectFinder<USkeletalMesh> Smesh{ (TEXT("/Script/Engine.SkeletalMesh'/Game/Armor/Lower/Armor_230_Lower.Armor_230_Lower'")) };
		Lower = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Lower"));
		Lower->SetupAttachment(RootComponent);
		Lower->SetRelativeLocation(Locate);
		Lower->SetRelativeScale3D(Scale);
		Lower->SetSkeletalMeshAsset(Smesh.Object);
	}

	{
		ConstructorHelpers::FObjectFinder<USkeletalMesh> Smesh{ (TEXT("/Script/Engine.SkeletalMesh'/Game/Armor/Upper/Armor_230_Upper.Armor_230_Upper'")) };
		Upper = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Upper"));
		Upper->SetupAttachment(RootComponent);
		Upper->SetRelativeLocation(Locate);
		Upper->SetRelativeScale3D(Scale);
		Upper->SetSkeletalMeshAsset(Smesh.Object);
	}

	{
		ConstructorHelpers::FObjectFinder<USkeletalMesh> Smesh{ (TEXT("/Script/Engine.SkeletalMesh'/Game/Armor/ArmorMix/ArmorMix.ArmorMix'")) };
		USkeletalMeshComponent* mMesh = GetMesh();
		mMesh->SetupAttachment(RootComponent);
		mMesh->SetRelativeLocation(Locate);
		mMesh->SetSkeletalMeshAsset(Smesh.Object);
		ConstructorHelpers::FClassFinder<UAnimInstance> Anim{ (TEXT("/Script/Engine.AnimBlueprint'/Game/Armor/BP_PlayerAnimInstance.BP_PlayerAnimInstance_C'")) };
		mMesh->SetAnimInstanceClass(Anim.Class);
		mMesh->SetRelativeScale3D(Scale);
	}
	{
		UCapsuleComponent* CC = GetCapsuleComponent();
		CC->SetCapsuleRadius(20.f);
		CC->SetCapsuleHalfHeight(41.f);

	}


	{	
		ConstructorHelpers::FObjectFinder<UAnimMontage> Asset(TEXT("/Script/Engine.AnimMontage'/Game/Armor/Animation/Sword_Attack/Sword_Attack_Charge_L_Montage.Sword_Attack_Charge_L_Montage'"));

		if (Asset.Object)
		{
			Sword_Attack_Charge_L_MTG = Asset.Object;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("No IMC_Character"));
		}
	}
	{
		LWeapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("LWeapon"));
		LWeapon->SetupAttachment(RootComponent);
		
		LWeapon->SetRelativeScale3D(Scale);

		RWeapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("RWeapon"));
		ConstructorHelpers::FObjectFinder<USkeletalMesh> Asset(TEXT("/Script/Engine.SkeletalMesh'/Game/Resource/Sword/Weapon_Sword_003.Weapon_Sword_003'"));
		RWeapon->SetSkeletalMeshAsset(Asset.Object);
		RWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("Weapon_R"));
	}

	bUseControllerRotationYaw = false;
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
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
	
	
	USkeletalMeshComponent* mMesh = GetMesh();
	RWeapon->SetLeaderPoseComponent(mMesh);
	LWeapon->SetLeaderPoseComponent(mMesh);
	Head->SetLeaderPoseComponent(mMesh);
	Lower->SetLeaderPoseComponent(mMesh);
	Upper->SetLeaderPoseComponent(mMesh);

}

