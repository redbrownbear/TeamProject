// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GameOver/GameOverUI.h"
#include "SubSystem/AsyncLoadingScreen/GIS_ASyncLoadingScreen.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PC_InGame.h"

void UGameOverUI::OnCreated()
{
	InitUI();
}

void UGameOverUI::ShowUI()
{
	Super::ShowUI();

	APC_InGame* PC_InGame = Cast<APC_InGame>(UGameplayStatics::GetPlayerController(this, 0));
	if (PC_InGame)
	{
		PC_InGame->ChangeInputContext(EInputContext::IC_UI);

		FInputModeUIOnly InputMode;
		InputMode.SetWidgetToFocus(TakeWidget());
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		PC_InGame->SetInputMode(InputMode);
		PC_InGame->bShowMouseCursor = true;
	}

	bIsFocusable = true;
	SetKeyboardFocus();

	SetRenderOpacity(0.0f);

	// 초기 숨김 설정
	if (ImageGameOver)
		ImageGameOver->SetRenderOpacity(0.0f);


	if (TextPressAnyKey)
		TextPressAnyKey->SetVisibility(ESlateVisibility::Hidden);

	// 타이머 등록
	GetWorld()->GetTimerManager().SetTimer(FadeThisHandle, this, &UGameOverUI::FadeInThis, 0.01f, true);
	GetWorld()->GetTimerManager().SetTimer(ImageFadeHandle, this, &UGameOverUI::StartImageFade, 1.0f, false);
	GetWorld()->GetTimerManager().SetTimer(ShowTextHandle, this, &UGameOverUI::ShowPressAnyKeyText, 3.0f, false);
}

void UGameOverUI::HideUI(TSubclassOf<UBaseUI> UIClass)
{
	Super::HideUI(UGameOverUI::StaticClass());

	UGIS_ASyncLoadingScreen* LoadingScreenSubsystem = GetGameInstance()->GetSubsystem<UGIS_ASyncLoadingScreen>();
	if (LoadingScreenSubsystem)
	{
		TSoftObjectPtr<UWorld> GameMap = TSoftObjectPtr<UWorld>(FSoftObjectPath(TEXT("/Game/Level/TitleMap.TitleMap")));
		LoadingScreenSubsystem->OpenLevelWithLoadingScreenGameOver(GameMap);
	}
}

FReply UGameOverUI::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	if (!bHasAlreadyTriggered)
	{
		bHasAlreadyTriggered = false;
		
		HideUI(UGameOverUI::StaticClass());
	}

	return FReply::Handled();
}

void UGameOverUI::InitUI()
{
	TextPressAnyKey->SetVisibility(ESlateVisibility::Collapsed);
	ImageGameOver->SetVisibility(ESlateVisibility::Collapsed);

	bHasAlreadyTriggered = true;
}

void UGameOverUI::FadeInThis()
{
	static float Time = 0.f;
	Time += 0.01f;

	float Alpha = FMath::Clamp(Time / 3.0f, 0.f, 1.f);
	SetRenderOpacity(Alpha);

	if (Alpha >= 1.0f)
	{
		GetWorld()->GetTimerManager().ClearTimer(FadeThisHandle);
		Time = 0.f;
	}
}

void UGameOverUI::StartImageFade()
{
	GetWorld()->GetTimerManager().SetTimer(ImageFadeTickHandle, this, &UGameOverUI::FadeInImage, 0.01f, true);
}

void UGameOverUI::FadeInImage()
{
	static float Time = 0.f;
	Time += 0.01f;

	float Alpha = FMath::Clamp(Time / 2.0f, 0.f, 1.f);
	if (ImageGameOver)
	{
		ImageGameOver->SetVisibility(ESlateVisibility::Visible);
		ImageGameOver->SetRenderOpacity(Alpha);
	}

	if (Alpha >= 1.0f)
	{
		GetWorld()->GetTimerManager().ClearTimer(ImageFadeTickHandle);
		Time = 0.f;
	}
}

void UGameOverUI::ShowPressAnyKeyText()
{
	if (TextPressAnyKey)
	{
		TextPressAnyKey->SetVisibility(ESlateVisibility::Visible);
		bHasAlreadyTriggered = false;
	}
}