// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Popup/PopupCountSelect.h"
#include "GameFramework/PC_InGame.h"
#include "Kismet/GameplayStatics.h"

void UPopupCountSelect::OnCreated()
{
	InitUI();
}

void UPopupCountSelect::ShowUI()
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

    BindDelegates();

    MaxCount = 0;
    CurrentCount = 1;
}

void UPopupCountSelect::HideUI(TSubclassOf<UBaseUI> UIClass)
{
    RemoveDelegate();
	Super::HideUI(UPopupCountSelect::StaticClass());
}

void UPopupCountSelect::InitUI()
{
    BtnMinus->OnClicked.AddDynamic(this, &UPopupCountSelect::OnMinusClick);
    BtnPlus->OnClicked.AddDynamic(this, &UPopupCountSelect::OnPlusClick);

    ButtonCancel->OnClicked.AddDynamic(this, &UPopupCountSelect::OnCancelClick);
}

void UPopupCountSelect::BindDelegates()
{
    UPlayerManager* PlayerManager = GetGameInstance()->GetSubsystem<UPlayerManager>();
    if (PlayerManager)
    {
        OnItemDropUpdated.AddDynamic(PlayerManager, &UPlayerManager::SetInvenRemoveData);
    }
}

void UPopupCountSelect::RemoveDelegate()
{
    UPlayerManager* PlayerManager = GetGameInstance()->GetSubsystem<UPlayerManager>();
    if (PlayerManager)
    {
        OnItemDropUpdated.RemoveDynamic(PlayerManager, &UPlayerManager::SetInvenRemoveData);
    }
}

void UPopupCountSelect::MoveSelection(FIntPoint Direction)
{
    if (Direction.X != 0) // аб©Л
    {
        CurrentCount += Direction.X;
    }

    CurrentCount = FMath::Clamp(CurrentCount, 1, MaxCount);

    TextCount->SetText(FText::FromString(FString::FromInt(CurrentCount)));
}

void UPopupCountSelect::SetUI(const FItemData& InItemData)
{
	ItemData = InItemData;
	TextCount->SetText(FText::FromString("1"));

    UPlayerManager* PlayerManager = GetGameInstance()->GetSubsystem<UPlayerManager>();
    check(PlayerManager);

    TArray<FItemData> TempItems = PlayerManager->GetAllItemData();

    for (FItemData Item : TempItems)
    {
        if ((Item.eItemCategory == EItemCategory::IT_Arrow && Item.bIsArrow) || Item.eItemCategory == EItemCategory::IT_Material)
        {
			if (Item.Name == InItemData.Name)
                MaxCount++;
        }
    }
}

void UPopupCountSelect::OnNavigate(const FInputActionValue& InputActionValue)
{
    UPlayerManager* PlayerManager = GetGameInstance()->GetSubsystem<UPlayerManager>();
    check(PlayerManager);

    const FVector2D ActionValue = InputActionValue.Get<FVector2D>();

    if (ActionValue.IsNearlyZero())
        return;

    if (FMath::Abs(ActionValue.X) > FMath::Abs(ActionValue.Y))
    {
        if (ActionValue.X > 0)
            MoveSelection(FIntPoint(1, 0));
        else
            MoveSelection(FIntPoint(-1, 0));
    }
}

void UPopupCountSelect::OnConfirm(const FInputActionValue& InputActionValue)
{
    UPlayerManager* PlayerManager = GetGameInstance()->GetSubsystem<UPlayerManager>();
    check(PlayerManager);
    TArray<FItemData> TempItems = PlayerManager->GetAllItemData();

    for (FItemData Item : TempItems)
    {
        if (CurrentCount <= 0)
            break;

        if ((Item.eItemCategory == EItemCategory::IT_Arrow && Item.bIsArrow) || Item.eItemCategory == EItemCategory::IT_Material)
        {
            if (Item.Name == ItemData.Name)
            {
                OnItemDropUpdated.Broadcast(Item);
                CurrentCount--;
            }
        }
    }

	OnCancel(FInputActionValue());
}

void UPopupCountSelect::OnCancel(const FInputActionValue& InputActionValue)
{
	HideUI(UPopupCountSelect::StaticClass());
}

void UPopupCountSelect::OnPlusClick()
{
    MoveSelection(FIntPoint(1, 0));
}

void UPopupCountSelect::OnMinusClick()
{
    MoveSelection(FIntPoint(-1, 0));
}

void UPopupCountSelect::OnCancelClick()
{
    OnCancel(FInputActionValue());
}

