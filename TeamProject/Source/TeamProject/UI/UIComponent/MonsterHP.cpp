// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UIComponent/MonsterHP.h"

void UMonsterHP::NativeConstruct()
{
	ProgressBarHp->SetVisibility(ESlateVisibility::Collapsed);
}

void UMonsterHP::ShowUI(float CurHp, float MaxHp)
{
	if (CurHp == MaxHp)
		return;

	ProgressBarHp->SetVisibility(ESlateVisibility::Visible);

	if (ProgressBarHp && MaxHp > 0.0f)
	{
		float HpPercent = CurHp / MaxHp;
		ProgressBarHp->SetPercent(HpPercent);
	}

	// 기존 타이머가 돌고 있으면 초기화
	if (GetWorld()->GetTimerManager().IsTimerActive(HideTimerHandle))
	{
		GetWorld()->GetTimerManager().ClearTimer(HideTimerHandle);
	}

	// 다시 5초 뒤에 숨김 예약
	GetWorld()->GetTimerManager().SetTimer(HideTimerHandle, this, &UMonsterHP::HideUI, 5.f, false);
}

void UMonsterHP::HideUI()
{
	ProgressBarHp->SetVisibility(ESlateVisibility::Hidden);
}
