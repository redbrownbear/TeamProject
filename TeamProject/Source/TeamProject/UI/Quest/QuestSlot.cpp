// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Quest/QuestSlot.h"

void UQuestSlot::NativeConstruct()
{
	BGHighlight->SetVisibility(ESlateVisibility::Collapsed);
}

void UQuestSlot::SetQuestData(const FQuestDataRow& InQuestData)
{
    //单捞磐 贸府 饶 UI 贸府
    QuestData = InQuestData;

    TextQuestName->SetText(FText::FromString(QuestData.QuestNPC));
    TextQuestPlace->SetText((FText::FromString(QuestData.QuestPlace)));
}

void UQuestSlot::SetSelected(bool bIsSelected)
{
	BGHighlight->SetVisibility(bIsSelected ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	BGNone->SetVisibility(bIsSelected ? ESlateVisibility::Hidden : ESlateVisibility::Visible);
}
