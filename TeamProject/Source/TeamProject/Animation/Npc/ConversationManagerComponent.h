// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ConversationManagerComponent.generated.h"

class ANpc;
class APlayerCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TEAMPROJECT_API UConversationManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UConversationManagerComponent();

public:
	void StartConversation(ANpc* Npc, APlayerCharacter* Player);
	void EndConversation(ANpc* Npc, APlayerCharacter* Player);

protected:
	virtual void BeginPlay() override;

public:
	void PlayTalkAnimations();
	void ShowTalkUI();
	void LockCharacters(ANpc* Npc, APlayerCharacter* Player);
	void UnlockCharacters(ANpc* Npc, APlayerCharacter* Player);

private:
	UPROPERTY()
	ANpc* CurrentNpc = nullptr;

	UPROPERTY()
	APlayerCharacter* CurrentPlayer = nullptr;

private:
	UPROPERTY()
	UAnimMontage* NpcTalkMontage = nullptr;

	UPROPERTY()
	UAnimMontage* PlayerTalkMontage = nullptr;
};
