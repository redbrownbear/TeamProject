// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/HUDQuestOn.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/CanvasPanelSlot.h"

FVector2D UHUDQuestOn::OriginPosition = FVector2D(700.0f, 150.0f);

void UHUDQuestOn::NativeConstruct()
{
	Super::NativeConstruct();

	if (bHasInitialized == false)
	{
		InitResource();
		bHasInitialized = true;
	}

	SetVisibility(ESlateVisibility::Collapsed);
}

void UHUDQuestOn::InitResource()
{
	QuestOn = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, TEXT("/Game/Resources/UI/HUD/HUD_QuestOn.HUD_QuestOn")));
	QuestCoplete = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, TEXT("/Game/Resources/UI/HUD/HUD_QuestComplete.HUD_QuestComplete")));
}

void UHUDQuestOn::ShowQuestUI(bool bIsComplete, FString QuestName)
{
	SetVisibility(ESlateVisibility::Visible);

	if(bIsComplete)
		ImageQuestBg->SetBrushFromTexture(QuestCoplete);

	else
		ImageQuestBg->SetBrushFromTexture(QuestOn);

	TextQuestName->SetText(FText::FromString(QuestName));


	PlaySlideIn();
}

void UHUDQuestOn::PlaySlideIn()
{
	if (!BoxQuest) return;

	if (UCanvasPanelSlot* PanelSlot = Cast<UCanvasPanelSlot>(BoxQuest->Slot))
	{
		// 중앙 정렬
		PanelSlot->SetAnchors(FAnchors(0.5f, 0.5f));
		PanelSlot->SetAlignment(FVector2D(0.5f, 0.5f));

		// 중앙 위치 저장
		OriginPosition = FVector2D(0.f, 0.f);
		BoxQuest->SetRenderTranslation(FVector2D(900.0f, 0.0f));

		// 슬라이드 인 시작
		GetWorld()->GetTimerManager().SetTimer(SlideInHandle, this, &UHUDQuestOn::AnimateToOrigin, 0.01f, true);
	}

	GetWorld()->GetTimerManager().SetTimer(SlideInHandle, this, &UHUDQuestOn::AnimateToOrigin, 0.01f, true);
}

void UHUDQuestOn::AnimateToOrigin()
{
	FVector2D Current = BoxQuest->RenderTransform.Translation;
	FVector2D Target = OriginPosition;
	FVector2D NewPos = FMath::Vector2DInterpTo(Current, Target, GetWorld()->GetDeltaSeconds(), 10.0f);
	BoxQuest->SetRenderTranslation(NewPos);

	if (FVector2D::Distance(NewPos, Target) < 1.0f)
	{
		GetWorld()->GetTimerManager().ClearTimer(SlideInHandle);

		GetWorld()->GetTimerManager().SetTimer(HideTimerHandle, this, &UHUDQuestOn::PlaySlideOut, 3.0f, false);
	}
}

void UHUDQuestOn::PlaySlideOut()
{
	GetWorld()->GetTimerManager().SetTimer(SlideOutHandle, this, &UHUDQuestOn::AnimateOut, 0.01f, true);
}

void UHUDQuestOn::AnimateOut()
{
	FVector2D Current = BoxQuest->RenderTransform.Translation;
	FVector2D Target = FVector2D(OriginPosition.X - 1840.0f, OriginPosition.Y);
	FVector2D NewPos = FMath::Vector2DInterpTo(Current, Target, GetWorld()->GetDeltaSeconds(), 10.0f);
	BoxQuest->SetRenderTranslation(NewPos);

	if (FVector2D::Distance(NewPos, Target) < 1.0f)
	{
		GetWorld()->GetTimerManager().ClearTimer(SlideOutHandle);

		// 위치 원상복구
		BoxQuest->SetRenderTranslation(OriginPosition);
		SetVisibility(ESlateVisibility::Collapsed);
	}
}
