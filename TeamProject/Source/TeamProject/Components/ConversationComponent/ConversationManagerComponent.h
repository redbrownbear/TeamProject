// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Actors/Npc/Npc.h"
#include "Data/NPCDialogueTableRow.h"
#include "ConversationManagerComponent.generated.h"

class APlayerCharacter;
class UQuestDialogueManager;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TEAMPROJECT_API UConversationManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UConversationManagerComponent();

public:
	void StartConversation(ANpc* Npc, APlayerCharacter* Player);
	void EndConversation();
	int32 GetDialogueID(UDataTable* DialogueTable, EQuestCharacter Character, EDialogType DialogType);

protected:
	virtual void BeginPlay() override;

public:
	void PlayTalkAnimations();
	void LockCharacters(ANpc* Npc, APlayerCharacter* Player);
	void UnlockCharacters(ANpc* Npc, APlayerCharacter* Player);

	void SetEndTalked(bool IsEndTalked) { bEndTalk = IsEndTalked; }
	bool GetEndTalked() { return bEndTalk; }
	UDataTable* GetDataTable() { return NpcDialogueTable; }

private:
	UPROPERTY()
	ANpc* CurrentNpc = nullptr;

	UPROPERTY()
	APlayerCharacter* CurrentPlayer = nullptr;

	UPROPERTY()
	UQuestDialogueManager* QuestDialogueManager = nullptr;

	UPROPERTY()
	FNPCDialogueTableRow DialogueDataRow;

private:
	UPROPERTY()
	UAnimMontage* NpcTalkMontage = nullptr;

	UPROPERTY()
	UAnimMontage* PlayerTalkMontage = nullptr;

	UPROPERTY()
	UDataTable* NpcDialogueTable = nullptr;

private:
	bool bEndTalk = false;

};
