// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Popup/PopupGetItem.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PC_InGame.h"

void UPopupGetItem::OnCreated()
{

}

void UPopupGetItem::ShowUI()
{
	Super::ShowUI();

	APC_InGame* PC_InGame = Cast<APC_InGame>(UGameplayStatics::GetPlayerController(this, 0));
	if (PC_InGame)
	{
		PC_InGame->ChangeInputContext(EInputContext::IC_UI);

		FInputModeGameAndUI InputMode;
		InputMode.SetWidgetToFocus(TakeWidget());
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		InputMode.SetHideCursorDuringCapture(false);

		PC_InGame->SetInputMode(InputMode);
	}

	InitUI();
}

void UPopupGetItem::HideUI(TSubclassOf<UBaseUI> UIClass)
{
	Super::HideUI(UPopupGetItem::StaticClass());
}

void UPopupGetItem::InitUI()
{

}

void UPopupGetItem::ShowData(FItemData ItemData)
{
	UTexture2D* LoadedTextureIcon = ItemData.ItemIcon.LoadSynchronous();
	if (LoadedTextureIcon)
	{
		ImgItemIcon->SetBrushFromTexture(LoadedTextureIcon);
	}

	UTexture2D* LoadedTextureCategory = ItemData.CategoryIcon.LoadSynchronous();
	if (LoadedTextureCategory)
	{
		ImgItemCategory->SetBrushFromTexture(LoadedTextureCategory);
	}

	TextItemName->SetText(FText::FromString(ItemData.Name));

	TextItemValue->SetText(FText::FromString(FString::FromInt(ItemData.Damage)));

	TextItemDescription->SetText(FText::FromString(ItemData.Description));
}

void UPopupGetItem::OnCancel()
{
	HideUI(UPopupGetItem::StaticClass());
}
