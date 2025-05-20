#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "NpcController.generated.h"

class UNpcFSMComponent;
class UConversationManagerComponent;

UCLASS()
class TEAMPROJECT_API ANpcController : public AAIController
{
	GENERATED_BODY()

public:
	ANpcController();

	virtual void Tick(float DeltaTime) override;

public:
	UConversationManagerComponent* GetConversationManager() const { return ConversationManager;  }

protected:
	virtual void BeginPlay() override;

protected:
	UFUNCTION()
	void OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors);
	UFUNCTION()
	void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

protected:
	UPROPERTY()
	TObjectPtr<UNpcFSMComponent> NpcFSMComponent;
	
	UPROPERTY()
	TObjectPtr<UConversationManagerComponent> ConversationManager;

public:
	bool bPerceive = false;
	bool bTalk = false; 
};
