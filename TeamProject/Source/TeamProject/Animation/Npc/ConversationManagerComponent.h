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
	void EndConversation();

protected:
	virtual void BeginPlay() override;

protected:
	void PlayTalkAnimations();
	void ShowTalkUI();
	void LockCharacters();
	void UnlockCharacters();

private:
	UPROPERTY()
	ANpc* CurrentNpc;

	UPROPERTY()
	APlayerCharacter* CurrentPlayer;
		
};
