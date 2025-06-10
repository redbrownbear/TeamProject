// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Quest/Quest.h"
#include "Kismet/GameplayStatics.h"

#include "SubSystem/PlayerManager.h"

#include "GameFramework/PC_InGame.h"
#include "UI/HUD/MainHUD.h"

void UQuest::OnCreated()
{
    InitializePool(50);
}

void UQuest::ShowUI()
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
    SetRupeeUI();
    BindDelegates();
}

void UQuest::HideUI(TSubclassOf<UBaseUI> UIClass)
{
    RemoveDelegates();
    Super::HideUI(UQuest::StaticClass());
}

void UQuest::InitUI()
{
}

void UQuest::InitializePool(int32 PreloadCount)
{
    for (int32 i = 0; i < PreloadCount; ++i)
    {
        UQuestSlot* PoolSlot = CreateWidget<UQuestSlot>(GetWorld(), SlotWidgetClass);
        PoolSlot->RemoveFromParent();
        PooledSlots.Add(PoolSlot);
    }
}

void UQuest::BindDelegates()
{
    UPlayerManager* PlayerManager = GetGameInstance()->GetSubsystem<UPlayerManager>();
    if (PlayerManager)
    {
        PlayerManager->OnQuestUpdated.AddDynamic(this, &UQuest::RefreshQuestList);
    }
}

void UQuest::RemoveDelegates()
{
    UPlayerManager* PlayerManager = GetGameInstance()->GetSubsystem<UPlayerManager>();
    if (PlayerManager)
    {
        PlayerManager->OnQuestUpdated.RemoveDynamic(this, &UQuest::RefreshQuestList);
    }
}

void UQuest::SetRupeeUI()
{
    UPlayerManager* PlayerManager = GetGameInstance()->GetSubsystem<UPlayerManager>();
    if (PlayerManager)
    {
        TextCoin->SetText(FText::FromString(FString::FromInt(PlayerManager->GetRupee())));
    }
}
void UQuest::RefreshQuestList(const TArray<FQuestDataRow>& QuestList)
{
    for (UQuestSlot* ActiveSlot : ActiveSlots)
    {
        ActiveSlot->RemoveFromParent();
        PooledSlots.Add(ActiveSlot);
    }
    ActiveSlots.Empty();

    for (const FQuestDataRow& QuestRow : QuestList)
    {
        UQuestSlot* NewSlot = nullptr;

        if (PooledSlots.Num() > 0)
        {
            NewSlot = PooledSlots.Pop();
        }
        else
        {
            NewSlot = CreateWidget<UQuestSlot>(GetWorld(), SlotWidgetClass);
        }

        NewSlot->SetQuestData(QuestRow);
        ItemVerticalBox->AddChildToVerticalBox(NewSlot);
        ActiveSlots.Add(NewSlot);
    }

    SelectInit();
    RefreshQuestDescription();
}

void UQuest::RefreshQuestDescription()
{
    FQuestDataRow Data = GetItemDataAtIndex(CurrentIndex);

    BP_QuestDescription->RefreshUI(Data);
}

void UQuest::MoveSelection(FIntPoint Direction)
{
    if (ActiveSlots.Num() == 0) return;

    const int32 MaxIndex = ActiveSlots.Num() - 1;

    int32 NextIndex = CurrentIndex;

    //좌우 제거

    if (Direction.Y != 0) // 상하
    {
        NextIndex += Direction.Y;
    }

    NextIndex = FMath::Clamp(NextIndex, 0, MaxIndex);

    if (NextIndex != CurrentIndex)
    {
        ScrollBox->ScrollWidgetIntoView(ActiveSlots[NextIndex], true, EDescendantScrollDestination::IntoView);
        ActiveSlots[CurrentIndex]->SetSelected(false);
        ActiveSlots[NextIndex]->SetSelected(true);
        CurrentIndex = NextIndex;
    }

    RefreshQuestDescription();
}

void UQuest::SelectInit()
{
    if (ActiveSlots.IsEmpty())
        return;

    for (UQuestSlot* slot : ActiveSlots)
    {
        slot->SetSelected(false);
    }

    CurrentIndex = 0;
    ActiveSlots[CurrentIndex]->SetSelected(true);

}

FQuestDataRow UQuest::GetItemDataAtIndex(int32 Index) const
{
    if (ActiveSlots.IsValidIndex(Index) && ActiveSlots[Index] != nullptr)
    {
        return ActiveSlots[Index]->GetQuestData();
    }

    return FQuestDataRow();
}

void UQuest::OnNavigate(const FInputActionValue& InputActionValue)
{
    const FVector2D ActionValue = InputActionValue.Get<FVector2D>();

    // Deadzone 방지
    if (ActionValue.IsNearlyZero())
        return;

    // 가장 강한 방향 하나만 해석
    if (FMath::Abs(ActionValue.X) > FMath::Abs(ActionValue.Y))
    {
        // 좌우
        if (ActionValue.X > 0)
            MoveSelection(FIntPoint(1, 0));
        else
            MoveSelection(FIntPoint(-1, 0));
    }
    else
    {
        // 상하
        if (ActionValue.Y > 0)
            MoveSelection(FIntPoint(0, -1));
        else
            MoveSelection(FIntPoint(0, 1));
    }
}

void UQuest::OnConfirm()
{
}

void UQuest::OnCancel()
{
    HideUI(UQuest::StaticClass());
}
