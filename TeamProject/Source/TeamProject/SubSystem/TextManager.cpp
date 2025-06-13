// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/TextDataRow.h"
#include "SubSystem/TextManager.h"

UTextManager::UTextManager()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> TextTableAsset(TEXT("/Game/Data/Text/DT_TextData.DT_TextData"));
	if (TextTableAsset.Succeeded())
	{
		TextDataTable = TextTableAsset.Object;
	}
}

FString UTextManager::GetTextByID(int32 TextID) const
{
	if (!TextDataTable)
		return TEXT(" ");

	TArray<FTextData*> AllRows;
	TextDataTable->GetAllRows(TEXT("TextLookup"), AllRows);

	for (FTextData* Row : AllRows)
	{
		if (Row && Row->TextID == TextID)
		{
			return Row->Text;
		}
	}

	return TEXT(" ");
}
