// Fill out your copyright notice in the Description page of Project Settings.


#include "SubSystem/AsyncLoadingScreen/GIS_ASyncLoadingScreen.h"
#include "Blueprint/UserWidget.h"
#include "MoviePlayer.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/StreamableManager.h"
#include "Engine/AssetManager.h"

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
		ensureMsgf(false, TEXT("WidgetClass is nullptr"));
		return;
	}

	ULoadingWithPlayerInfo* Widget = CreateWidget<ULoadingWithPlayerInfo>(GetWorld(), LoadingWidgetClass);


	if (Widget && IsMoviePlayerEnabled() && FSlateApplication::IsInitialized())
	{
		FLoadingScreenAttributes LoadingScreenAttributes;
		LoadingScreenAttributes.WidgetLoadingScreen = Widget->TakeWidget();
		LoadingScreenAttributes.MinimumLoadingScreenDisplayTime = 3.f;
		LoadingScreenAttributes.bAutoCompleteWhenLoadingCompletes = true;
		LoadingScreenAttributes.bAllowEngineTick = true;

		GetMoviePlayer()->SetupLoadingScreen(LoadingScreenAttributes);
		GetMoviePlayer()->PlayMovie();
	}

	//진척도 확인을 위함
	UPlayerManager* PlayerManager = GetGameInstance()->GetSubsystem<UPlayerManager>();
	if (PlayerManager)
	{
		PlayerManager->SetTargetLevel(Level);
	}

	FStreamableManager& Streamable = UAssetManager::GetStreamableManager();
	Streamable.RequestAsyncLoad(Level.ToSoftObjectPath(), FStreamableDelegate::CreateLambda([this, Level]()
		{
			UE_LOG(LogTemp, Log, TEXT("Async load complete, opening level..."));
			UGameplayStatics::OpenLevelBySoftObjectPtr(this, Level);
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
