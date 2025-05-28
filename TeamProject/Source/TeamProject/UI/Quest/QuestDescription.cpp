// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Quest/QuestDescription.h"

void UQuestDescription::RefreshUI(const FQuestDataRow& QuestData)
{
	check(DescriptionTitle);
	check(TextNPC);
	check(TextPlace);
	check(TextDescriptionBox);

	DescriptionTitle->SetText(FText::FromString(QuestData.QuestTitle));
	TextNPC->SetText((FText::FromString(QuestData.QuestNPC)));
	TextPlace->SetText((FText::FromString(QuestData.QuestPlace)));
	TextDescriptionBox->SetText((FText::FromString(QuestData.QuestDescription)));
}