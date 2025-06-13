// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/TitlePlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

void ATitlePlayerController::BeginPlay()
{
	if (TitleWidgetClass)
	{
		TitleWidget = CreateWidget<UTitleWidget>(this, TitleWidgetClass);
		if (TitleWidget)
		{
			TitleWidget->AddToViewport();

			// UI �Է� ��� ����
			FInputModeUIOnly InputMode;
			InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			SetInputMode(InputMode);
			bShowMouseCursor = true;
		}
	}
}

void ATitlePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// UI�� �޵��� ����
	FInputModeUIOnly InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	SetInputMode(InputMode);
	bShowMouseCursor = true;
}

void ATitlePlayerController::KeyBind()
{
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	Subsystem->AddMappingContext(TitleIMC, 0);
	SetInputMode(FInputModeGameOnly());
	bShowMouseCursor = false;

	if (UEnhancedInputComponent* EnhancedInputComp = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComp->BindAction(NavigateInputAction, ETriggerEvent::Completed, this, &ATitlePlayerController::OnNavigate);
		EnhancedInputComp->BindAction(ConfirmInputAction, ETriggerEvent::Completed, this, &ATitlePlayerController::OnConfirm);
	}
}

void ATitlePlayerController::OnNavigate(const FInputActionValue& InputActionValue)
{
	// �Է� �� �������� (��: 2D Vector - �����е� �Ƴ��α� ��ƽ, ����Ű ��)
	FVector2D Direction = InputActionValue.Get<FVector2D>();

	if (TitleWidget)
	{
		TitleWidget->OnNavigate(InputActionValue);
	}
}

void ATitlePlayerController::OnConfirm()
{
	if (TitleWidget)
	{
		TitleWidget->OnConfirm();
	}
}
