// Fill out your copyright notice in the Description page of Project Settings.


#include "Kismet/GameplayStatics.h"

#include "GameMode/UITestGameMode.h"

#include "SubSystem//UI/UIManager.h"
#include "UI/Test/TestPopupUI.h"


void AUITestGameMode::BeginPlay()
{
    Super::BeginPlay();

    CreatePopup();
}

void AUITestGameMode::CreatePopup()
{
    APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
    if (PC)
    {
        FInputActionBinding& Bind = PC->InputComponent->BindAction("OpenPopup", IE_Pressed, this, &AUITestGameMode::OnOpenPopup);
    }
}

void AUITestGameMode::OnOpenPopup()
{
    UUIManager* UIManager = GetGameInstance()->GetSubsystem<UUIManager>();
    check(UIManager);

    if (UIManager)
    {
        UTestPopupUI* NewUI = UIManager->CreatePopup(GetWorld());
        if (NewUI)
        {
            FText Title = FText::FromString(TEXT("Hi"));
            FText Main = FText::FromString(TEXT("Welcome"));

            NewUI->SetPopupText(Title, Main);
        }
    }
}
