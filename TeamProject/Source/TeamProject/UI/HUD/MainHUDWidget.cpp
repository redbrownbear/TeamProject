// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/MainHUDWidget.h"

#include "SubSystem/PlayerManager.h"

void UMainHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if(HIntOverlay)
		HIntOverlay->SetVisibility(ESlateVisibility::Hidden);

	if (NameOvelay)
		NameOvelay->SetVisibility(ESlateVisibility::Hidden);

	if (AimingBowOverlay)
		AimingBowOverlay->SetVisibility(ESlateVisibility::Hidden);

	if (AimingAbilityOverlay)
		AimingAbilityOverlay->SetVisibility(ESlateVisibility::Hidden);

	if (BossHpOverlay)
		BossHpOverlay->SetVisibility(ESlateVisibility::Hidden);

	InitValue();
	InitResource();
}

void UMainHUDWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	UpdateStamina(InDeltaTime);
}

void UMainHUDWidget::InitValue()
{
	ElapsedTime = 0.0f;
	StaminaHideDelayTime = 0.0f;
	StaminaRegenSpeed = 1.0f;
	bIsStaminaVisible = true;
}

void UMainHUDWidget::InitResource()
{
	HpFull = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, TEXT("/Game/Resources/UI/HUD/Heart_Full.Heart_Full")));
	Hp075 = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, TEXT("/Game/Resources/UI/HUD/Heart_075.Heart_075")));
	HpHalf = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, TEXT("/Game/Resources/UI/HUD/Heart_Half.Heart_Half")));
	Hp025 = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, TEXT("/Game/Resources/UI/HUD/Heart_025.Heart_025")));
	HpEmpty = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, TEXT("/Game/Resources/UI/HUD/Heart_Empty.Heart_Empty")));

	if (StaminaImg)
	{
		DynamicMaterial = DynamicMaterial = StaminaImg->GetDynamicMaterial();
	}
}

void UMainHUDWidget::ShowInteractUI(bool bShow)
{
	if (HIntOverlay)
		HIntOverlay->SetVisibility(bShow ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void UMainHUDWidget::ShowInteractName(bool bIsShow, FString Name)
{
	if (NameOvelay)
		NameOvelay->SetVisibility(bIsShow ? ESlateVisibility::Visible : ESlateVisibility::Hidden);

	if (bIsShow)
	{
		FText NametoText = FText::FromString(Name);
		NameText->SetText(NametoText);
	}
}

void UMainHUDWidget::ShowBowAimgUI(bool bIsShow, int32 Count)
{
	if (AimingBowOverlay)
		AimingBowOverlay->SetVisibility(bIsShow ? ESlateVisibility::Visible : ESlateVisibility::Hidden);

	if (TextArrowCount)
	{
		TextArrowCount->SetText(FText::FromString(FString::FromInt(Count)));
	}

}

void UMainHUDWidget::ShowAbilityAimUI(bool bIsShow)
{
	if (AimingAbilityOverlay)
		AimingAbilityOverlay->SetVisibility(bIsShow ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void UMainHUDWidget::ShowBossHP(bool bIsShow, float Hp, float MaxHp, FString Name)
{
	if(BossHpOverlay)
		BossHpOverlay->SetVisibility(bIsShow ? ESlateVisibility::Visible : ESlateVisibility::Hidden);

	if(TextBossName)
		TextBossName->SetText(FText::FromString(Name));

	if (BossHpProgressBar && MaxHp > 0.0f)
	{
		float HpPercent = Hp / MaxHp;
		BossHpProgressBar->SetPercent(HpPercent);
	}
}

void UMainHUDWidget::ShowQuestOn(bool bIsComplete, FString Name)
{
	QuestOn->ShowQuestUI(bIsComplete, Name);
}

void UMainHUDWidget::UpdateHP()
{
	UPlayerManager* PlayerManager = GetGameInstance()->GetSubsystem<UPlayerManager>();
	check(PlayerManager);

	int32 CurrentHP = PlayerManager->GetHp();
	int32 MaxHP = PlayerManager->GetMaxHp();

	int32 MaxHeartCount = FMath::CeilToInt(MaxHP / 4.0f);

	if (HeartImages.Num() != MaxHeartCount)
	{
		for (UImage* Image : HeartImages)
		{
			if (Image && Image->IsValidLowLevel())
			{
				Image->RemoveFromParent();
			}
		}
		HeartImages.Empty();

		for (int32 i = 0; i < MaxHeartCount; ++i)
		{
			UImage* NewHeart = NewObject<UImage>(this);
			NewHeart->SetVisibility(ESlateVisibility::Visible);

			HeartImages.Add(NewHeart);

			if (HpWrapBox)
			{
				HpWrapBox->AddChild(NewHeart);
			}
		}
	}

	for (int32 i = 0; i < MaxHeartCount; ++i)
	{
		int32 HeartStartHp = i * 4;
		int32 RemainingHP = FMath::Clamp(CurrentHP - HeartStartHp, 0, 4);

		UTexture2D* SelectedTex = nullptr;
		switch (RemainingHP)
		{
		case 4: SelectedTex = HpFull; break;
		case 3: SelectedTex = Hp075; break;
		case 2: SelectedTex = HpHalf; break;
		case 1: SelectedTex = Hp025; break;
		default: SelectedTex = HpEmpty; break;
		}

		if (HeartImages.IsValidIndex(i) && SelectedTex)
		{
			FSlateBrush Brush;
			Brush.SetResourceObject(SelectedTex);
			Brush.ImageSize = FVector2D(50.f, 40.f);

			HeartImages[i]->SetBrush(Brush);
		}
	}
}

void UMainHUDWidget::UpdateStamina(float InDeltaTime)
{
	UPlayerManager* PlayerManager = GetGameInstance()->GetSubsystem<UPlayerManager>();
	if (!PlayerManager) 
		return;

	float Current = PlayerManager->GetStamina();
	float Max = PlayerManager->GetMaxStamina();
	bool bFull = PlayerManager->IsStaminaFull();

	float FillRatio = FMath::Clamp(Current / Max, 0.0f, 1.0f);
	if (DynamicMaterial)
	{
		DynamicMaterial->SetScalarParameterValue("Percent", FillRatio);
	}

	if (bFull)
	{
		if (bIsStaminaVisible && !GetWorld()->GetTimerManager().IsTimerActive(StaminaHideTimerHandle))
		{
			// Å¸ÀÌ¸Ó ½ÃÀÛ: 1ÃÊ ÈÄ ¼û±è
			GetWorld()->GetTimerManager().SetTimer(
				StaminaHideTimerHandle,
				this,
				&UMainHUDWidget::HideStamina,
				1.0f,
				false
			);
		}
	}
	else
	{
		if (!bIsStaminaVisible)
		{
			StaminaImg->SetVisibility(ESlateVisibility::Visible);
			bIsStaminaVisible = true;
		}

		GetWorld()->GetTimerManager().ClearTimer(StaminaHideTimerHandle);
	}
}

void UMainHUDWidget::HideStamina()
{
	if (StaminaImg)
	{
		StaminaImg->SetVisibility(ESlateVisibility::Hidden);
		bIsStaminaVisible = false;
	}
}
