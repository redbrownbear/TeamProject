// Fill out your copyright notice in the Description page of Project Settings.


#include "SubSystem/AsyncLoadingScreen/GIS_ASyncLoadingScreen.h"
#include "Blueprint/UserWidget.h"
#include "MoviePlayer.h"
#include "Kismet/GameplayStatics.h"


void UGIS_ASyncLoadingScreen::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UGIS_ASyncLoadingScreen::ShowLoadingScreen(TSubclassOf<UUserWidget> WidgetClass)
{
	UUserWidget* UserWidget = CreateWidget<UUserWidget>(GetWorld(), WidgetClass);
	UserWidget->AddToViewport();
	if (IsMoviePlayerEnabled())
	{
		FLoadingScreenAttributes LoadingScreenAttributes;
		LoadingScreenAttributes.WidgetLoadingScreen = UserWidget->TakeWidget();
		//LoadingScreenAttributes.WidgetLoadingScreen = FLoadingScreenAttributes::NewTestLoadingScreenWidget();
		LoadingScreenAttributes.MinimumLoadingScreenDisplayTime = 3.f;
		LoadingScreenAttributes.bAutoCompleteWhenLoadingCompletes = true;
		LoadingScreenAttributes.bAllowEngineTick = true;

		GetMoviePlayer()->SetupLoadingScreen(LoadingScreenAttributes);
	}
}

void UGIS_ASyncLoadingScreen::OpenLevelWithLoadingScreen(TSubclassOf<UUserWidget> WidgetClass, const TSoftObjectPtr<UWorld> Level)
{
	if (!WidgetClass)
	{
		ensureMsgf(false, TEXT("WidgetClass is nullptr"));
		return;
	}
	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), WidgetClass);

	if (IsMoviePlayerEnabled())
	{
		FLoadingScreenAttributes LoadingScreenAttributes;
		LoadingScreenAttributes.WidgetLoadingScreen = Widget->TakeWidget();
		//LoadingScreenAttributes.WidgetLoadingScreen = FLoadingScreenAttributes::NewTestLoadingScreenWidget();
		LoadingScreenAttributes.MinimumLoadingScreenDisplayTime = 3.f;
		LoadingScreenAttributes.bAutoCompleteWhenLoadingCompletes = true;
		LoadingScreenAttributes.bAllowEngineTick = true;

		GetMoviePlayer()->SetupLoadingScreen(LoadingScreenAttributes);
	}

	UGameplayStatics::OpenLevelBySoftObjectPtr(this, Level);
}
