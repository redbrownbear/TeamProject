// Fill out your copyright notice in the Description page of Project Settings.


#include "SubSystem/AsyncLoadingScreen/GIS_ASyncLoadingScreen.h"
#include "Blueprint/UserWidget.h"
#include "MoviePlayer.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/StreamableManager.h"
#include "Engine/AssetManager.h"
#include "Components/Overlay.h"
#include "Blueprint/WidgetTree.h"

#include "SubSystem/PlayerManager.h"


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

void UGIS_ASyncLoadingScreen::OpenLevelWithLoadingScreenTitle(TSubclassOf<UUserWidget> WidgetClass, const TSoftObjectPtr<UWorld> Level)
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
		LoadingScreenAttributes.MinimumLoadingScreenDisplayTime = 3.f;
		LoadingScreenAttributes.bAutoCompleteWhenLoadingCompletes = true;
		LoadingScreenAttributes.bAllowEngineTick = true;

		GetMoviePlayer()->SetupLoadingScreen(LoadingScreenAttributes);
		GetMoviePlayer()->PlayMovie();
	}

	FStreamableManager& Streamable = UAssetManager::GetStreamableManager();
	Streamable.RequestAsyncLoad(Level.ToSoftObjectPath(), FStreamableDelegate::CreateLambda([this, Level]()
		{
			UGameplayStatics::OpenLevelBySoftObjectPtr(this, Level);
		}));
}

void UGIS_ASyncLoadingScreen::OpenLevelWithLoadingScreen(const TSoftObjectPtr<UWorld> Level)
{
	if (!LoadingWidgetClass)
	{
		ensureMsgf(false, TEXT("LoadingWidgetClass is nullptr"));
		return;
	}

	if (LoadingWidget)
	{
		LoadingWidget->RemoveFromParent();
		LoadingWidget = nullptr;
	}

	ULoadingWithPlayerInfo* Widget = CreateWidget<ULoadingWithPlayerInfo>(GetWorld(), LoadingWidgetClass);
	if (Widget)
	{
		Widget->AddToViewport();
		LoadingWidget = Widget;
	}

	SetPendingLevel(Level);

	FStreamableManager& Streamable = UAssetManager::GetStreamableManager();
	Streamable.RequestAsyncLoad(Level.ToSoftObjectPath(), FStreamableDelegate::CreateLambda([this, Level]()
		{
			GetWorld()->GetTimerManager().SetTimerForNextTick([this, Level]()
				{
					FTimerHandle TimerHandle;
					GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this, Level]()
						{
							UGameplayStatics::OpenLevel(this, TEXT("LoadingLevel"));
						}, 0.5f, false);
				});
		}));
}

void UGIS_ASyncLoadingScreen::OpenLevelWithLoadingScreenGameOVer(const TSoftObjectPtr<UWorld> Level)
{
	if (!LoadingWidgetToTitleClass)
	{
		ensureMsgf(false, TEXT("LoadingWidgetClass is nullptr"));
		return;
	}

	if (LoadingWidgetToTitle)
	{
		LoadingWidgetToTitle->RemoveFromParent();
		LoadingWidgetToTitle = nullptr;
	}

	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), LoadingWidgetToTitleClass);
	if (Widget)
	{
		Widget->AddToViewport();
		LoadingWidgetToTitle = Widget;
	}

	SetPendingLevel(Level);

	FStreamableManager& Streamable = UAssetManager::GetStreamableManager();
	Streamable.RequestAsyncLoad(Level.ToSoftObjectPath(), FStreamableDelegate::CreateLambda([this, Level]()
		{
			GetWorld()->GetTimerManager().SetTimerForNextTick([this, Level]()
				{
					FTimerHandle TimerHandle;
					GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this, Level]()
						{
							UGameplayStatics::OpenLevel(this, TEXT("LoadingLevel"));
						}, 0.5f, false);
				});
		}));
}

void UGIS_ASyncLoadingScreen::OpenLevelWithLoadingScreenNonAsynchronous(TSubclassOf<UUserWidget> WidgetClass, const TSoftObjectPtr<UWorld> Level)
{
	if (!WidgetClass)
	{
		ensureMsgf(false, TEXT("WidgetClass is nullptr"));
		return;
	}
	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), WidgetClass);
	Widget->AddToViewport();

	FTimerHandle TimerHandle;
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindLambda([this, Widget, Level]() 
		{
			if (Widget)
			{
				Widget->RemoveFromParent();
			}
			UGameplayStatics::OpenLevelBySoftObjectPtr(this, Level);
		});

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, 3.0f, false);
}

void UGIS_ASyncLoadingScreen::BeginLoading(TSoftObjectPtr<UWorld> LevelToOpen)
{
	UGameplayStatics::OpenLevelBySoftObjectPtr(this, LevelToOpen);
}
