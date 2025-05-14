// Fill out your copyright notice in the Description page of Project Settings.

#include "Kismet/GameplayStatics.h"

#include "SubSystem/UI/UIManager.h"
#include "UI/Test/TestPopupUI.h"

void UTestPopupUI::OnCreated()
{
	Super::OnCreated();

	//우선 보류
	//ExitButton->OnClicked.AddDynamic(this, &UTestPopupUI::OnMyButtonClicked);

	APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
	if (PC)
	{
		FInputActionBinding& Bind = PC->InputComponent->BindAction("ClosePopup", IE_Pressed, this, &UTestPopupUI::ClosePopup);
	}
}

void UTestPopupUI::OnMyButtonClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("My Button was clicked!"));
}

void UTestPopupUI::ClosePopup()
{
	if (!IsInViewport())
		return;

	UUIManager* UIManager = GetGameInstance()->GetSubsystem<UUIManager>();
	check(UIManager);

	if (UIManager)
	{
		UIManager->RemovePopup(this);
	}
}

void UTestPopupUI::SetPopupText(FText title, FText main)
{
	TitleText->SetText(title);
	MainText->SetText(main);
}
