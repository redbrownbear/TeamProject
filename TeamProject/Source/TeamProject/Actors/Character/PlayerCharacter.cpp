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
#include "Components/Character/PlayerMovementComponent.h"
#include "Kismet/GameplayStatics.h"

#include "SubSystem/PlayerManager.h"


// Sets default values
APlayerCharacter::APlayerCharacter(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer.SetDefaultSubobjectClass<UPlayerMovementComponent>(ACharacter::CharacterMovementComponentName))
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
		SpringArm->TargetArmLength = 300.f;
		SpringArm->SetRelativeLocation(FVector(0.f, 0.f, 30.f));
		

		{
			SpringArm->SetupAttachment(RootComponent);
			SpringArm->ProbeSize = 5.0;
			SpringArm->bUsePawnControlRotation = true;
			SpringArm->bInheritRoll = false;

			SpringArm->CameraLagSpeed = 5.f;
			SpringArm->CameraLagMaxDistance = 100.f;
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
		CC->SetCapsuleRadius(PLAYER_CAPSULE_RADIUS);
		CC->SetCapsuleHalfHeight(PLAYER_CAPSULE_HALF_HEIGHT);
		
	}

	{
		StatusComponent = CreateDefaultSubobject<UPlayerStatusComponent>(TEXT("PlayerStatus"));
	}
	
	ZoomTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("ZoomTimeline"));


	WeaponManagerComponent = CreateDefaultSubobject<UWeaponManagerComponent>(TEXT("WeaponManager"));
	bUseControllerRotationYaw = false;

	if (!ZoomCurve)
	{
		static ConstructorHelpers::FObjectFinder<UCurveFloat> CurveAsset(TEXT("/Script/Engine.CurveFloat'/Game/Resources/Player/Camera/CameraZoom.CameraZoom'")); // 예시 경로
		if (CurveAsset.Succeeded())
		{
			ZoomCurve = CurveAsset.Object;
		}
	}
	if (ZoomCurve)
	{
		InterpFunction.BindUFunction(this, FName("TimelineProgress"));
		ZoomTimeline->AddInterpFloat(ZoomCurve, InterpFunction);

	}
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	//Arrow->GetChildActor()->SetOwner(this);
	SpringArm->ProbeChannel = ECC_GameTraceChannel1;

	GetMesh()->SetCollisionProfileName(TEXT("Player"));

	UWorld* World = GetWorld();
	if (World)
	{
		// Get the current level name
		FString LevelName = World->GetName();
		if (LevelName.Equals(TEXT("GameMap")))
		{
			UPlayerManager* PlayerManager = GetGameInstance()->GetSubsystem<UPlayerManager>();

			FVector LinkLocation = PlayerManager->GetPlayerStatus().PreviousLoction;
			if (LinkLocation != FVector::Zero())
			{
				SetActorLocation(LinkLocation);
			}
		}
	}
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//스테미너 정보
	UPlayerManager* PlayerManager = GetGameInstance()->GetSubsystem<UPlayerManager>();
	if (PlayerManager)
	{
		PlayerManager->TickStamina(DeltaTime);
	}
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void APlayerCharacter::OnConstruction(const FTransform& Transform)
{
	
	

}

void APlayerCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	UPlayerMovementComponent* Movement = Cast<UPlayerMovementComponent>(GetCharacterMovement());
	Movement->SetGlideMode(false);
	GetMesh()->GetAnimInstance()->Montage_Play(Movement->GetGlideUnEquipMontage());
	

}

void APlayerCharacter::TimelineProgress(float Value)
{

	float Length = FMath::Lerp(300.f, 150.f, Value);

	FVector SpringArmLocation = FVector::Zero();
	float Z = FMath::Lerp(30.f, 50.f, Value);
	
	SpringArmLocation.Z = Z;
	SpringArm->SetRelativeLocation(SpringArmLocation);
	SpringArm->TargetArmLength = Length;
}







void APlayerCharacter::ZoomIn()
{
	if (!bZoomedIn)
	{
		bZoomedIn = true;
		ZoomTimeline->Play();
	}
}

void APlayerCharacter::ZoomOut()
{
	if (bZoomedIn)
	{
		bZoomedIn = false;
		ZoomTimeline->Reverse();
	}
}