// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Shop/ShopDescription.h"

void UShopDescription::RefreshUI(const FItemData& ItemData)
{
	UTexture2D* LoadedTexture = ItemData.ItemIcon.LoadSynchronous();
	if (LoadedTexture)
	{
		ItemImg->SetBrushFromTexture(LoadedTexture);
	}

	PriceText->SetText(FText::FromString(FString::FromInt(ItemData.price)));
	
	InfoTitle->SetText(FText::FromString(ItemData.Name));
	InfoDescription->SetText(FText::FromString(ItemData.Description));


	//이후로 가지고 있는 아이템 갯수
	//매니저를 통해 가져오자...

	CurrentItemData = ItemData; // 인벤토리에 데이터 전달하기 위한 함수
}
