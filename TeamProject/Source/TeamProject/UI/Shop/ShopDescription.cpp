// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Shop/ShopDescription.h"
#include "SubSystem/PlayerManager.h"

void UShopDescription::RefreshUI(const FItemData& ItemData)
{
	UPlayerManager* PlayerManager = GetWorld()->GetGameInstance()->GetSubsystem<UPlayerManager>();
	check(PlayerManager);

	UTexture2D* LoadedTexture = ItemData.ItemIcon.LoadSynchronous();
	if (LoadedTexture)
	{
		ItemImg->SetBrushFromTexture(LoadedTexture);
	}

	PriceText->SetText(FText::FromString(FString::FromInt(ItemData.price)));
	
	InfoTitle->SetText(FText::FromString(ItemData.Name));
	InfoDescription->SetText(FText::FromString(ItemData.Description));

	CountText->SetText(FText::FromString(FString::FromInt(PlayerManager->GetItemCountByName(ItemData.Name))));

	//CurrentItemData = ItemData; // 인벤토리에 데이터 전달하기 위한 함수
}
