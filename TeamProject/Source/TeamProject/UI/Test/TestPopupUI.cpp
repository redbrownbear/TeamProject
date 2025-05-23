// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Test/TestPopupUI.h"

#include "SubSystem/UI/UIManager.h"

void UTestPopupUI::ShowUI()
{
	Super::ShowUI();

	//우선 보류
	ExitButton->OnClicked.AddDynamic(this, &UTestPopupUI::OnMyButtonClicked);

	APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
	if (PC)
	{
		FInputActionBinding& Bind = PC->InputComponent->BindAction("ClosePopup", IE_Pressed, this, &UTestPopupUI::ClosePopup);
	}
}

void UTestPopupUI::OnMyButtonClicked()
{
	ClosePopup();
}

void UTestPopupUI::ClosePopup()
{
	if (!IsInViewport())
		return;

	HideUI(UTestPopupUI::StaticClass());
}

void UTestPopupUI::SetPopupText(FText title, FText main)
{
	TitleText->SetText(title);
	MainText->SetText(main);
}
