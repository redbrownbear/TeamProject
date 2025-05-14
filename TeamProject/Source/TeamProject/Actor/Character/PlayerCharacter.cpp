// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Character/PlayerCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/SkeletalMesh.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FVector Scale = FVector(45.f, 45.f, 45.f);
	FVector Locate = FVector(0.f, 0.f, -41.f);
	Head = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HEAD"));
	Head->SetupAttachment(RootComponent);
	Head->SetRelativeLocation(Locate);
	Head->SetRelativeScale3D(Scale);
	Head->SkeletalMesh;

	Lower = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Lower"));
	Lower->SetupAttachment(RootComponent);
	Lower->SetRelativeLocation(Locate);
	Lower->SetRelativeScale3D(Scale);

	USkeletalMeshComponent* mMesh = GetMesh();
	mMesh->SetupAttachment(RootComponent);
	mMesh->SetRelativeScale3D(Scale);
	mMesh->SetRelativeLocation(Locate);
	


	ConstructorHelpers::FObjectFinder<UAnimMontage> Asset(TEXT("/Script/Engine.AnimMontage'/Game/CharacterResourceTest/Animation/Sword/MTG_Lsword_Attack_Dash.MTG_Lsword_Attack_Dash'"));

	if (Asset.Object)
	{
		Lsword_Attack_Dash_MTG = Asset.Object;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No IMC_Character"));
	}
	LWeapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("LWeapon"));
	LWeapon->SetupAttachment(RootComponent);
	LWeapon->SetRelativeLocation(FVector(0.f, 0.f, -41.f));
	LWeapon->SetRelativeScale3D(Scale);

	RWeapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("RWeapon"));
	RWeapon->SetupAttachment(RootComponent);
	RWeapon->SetRelativeLocation(FVector(0.f, 0.f, -41.f));
	RWeapon->SetRelativeScale3D(Scale);
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


}

