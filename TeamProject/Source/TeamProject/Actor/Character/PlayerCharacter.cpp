// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Character/PlayerCharacter.h"
#include "Components/SkeletalMeshComponent.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FVector Scale = FVector(45.f, 45.f, 45.f);

	Head = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HEAD"));
	Head->SetupAttachment(RootComponent);
	Head->SetRelativeLocation(FVector(0.f, 0.f, -90.f));
	Head->SetRelativeScale3D(Scale);

	Lower = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Lower"));
	Lower->SetupAttachment(RootComponent);
	Lower->SetRelativeLocation(FVector(0.f, 0.f, -90.f));
	Lower->SetRelativeScale3D(Scale);

	USkeletalMeshComponent* mMesh = GetMesh();
	mMesh->SetRelativeScale3D(FVector(100.f, 100.f, 100.f));
	mMesh->SetRelativeLocation(Scale);
	

	ConstructorHelpers::FObjectFinder<UAnimMontage> Asset(TEXT("/Script/Engine.AnimMontage'/Game/CharacterResourceTest/Animation/Sword/MTG_Lsword_Attack_Dash.MTG_Lsword_Attack_Dash'"));

	if (Asset.Object)
	{
		Lsword_Attack_Dash_MTG = Asset.Object;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No IMC_Character"));
	}
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

