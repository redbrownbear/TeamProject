// Fill out your copyright notice in the Description page of Project Settings.


#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"
#include "UI/Title/TitleWidget.h"
#include "GameFramework/TitlePlayerController.h"

void UTitleWidget::NativeConstruct()
{
	Super::NativeConstruct();

	check(NewGame);

	NewGame->OnClicked.AddDynamic(this, &UTitleWidget::OnStartClicked);

	ImgContinue->SetVisibility(ESlateVisibility::Visible);
	ImgNewGame->SetVisibility(ESlateVisibility::Hidden);
	ImgExit->SetVisibility(ESlateVisibility::Hidden);

}

void UTitleWidget::OnStartClicked()
{
	UGameplayStatics::OpenLevel(this, FName("GameMap"));
}

void UTitleWidget::OnNavigate(const FInputActionValue& InputActionValue)
{

}

void UTitleWidget::OnConfirm()
{

}
