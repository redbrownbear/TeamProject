// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Actors/Npc/Npc.h"
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

protected:
	virtual void BeginPlay() override;

public:
	void PlayTalkAnimations();
	void LockCharacters(ANpc* Npc, APlayerCharacter* Player);
	void UnlockCharacters(ANpc* Npc, APlayerCharacter* Player);

	bool GetState() { return bStateChange; }

private:
	UPROPERTY()
	ANpc* CurrentNpc = nullptr;

	UPROPERTY()
	APlayerCharacter* CurrentPlayer = nullptr;

	UPROPERTY()
	UQuestDialogueManager* QuestDialogueManager = nullptr;

private:
	UPROPERTY()
	UAnimMontage* NpcTalkMontage = nullptr;

	UPROPERTY()
	UAnimMontage* PlayerTalkMontage = nullptr;

private:
	bool bStateChange = false;

};
