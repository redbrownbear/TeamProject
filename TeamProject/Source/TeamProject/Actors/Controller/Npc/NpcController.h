#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "NpcController.generated.h"

class UNpcFSMComponent;

UCLASS()
class TEAMPROJECT_API ANpcController : public AAIController
{
	GENERATED_BODY()

public:
	ANpcController();

	virtual void Tick(float DeltaTime) override;

public:
	

protected:
	virtual void BeginPlay() override;

protected:
	/*UFUNCTION()
	void OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors);*/
	UFUNCTION()
	void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

protected:
	UPROPERTY()
	TObjectPtr<UNpcFSMComponent> NpcFSMComponent;
	
public:
	bool bPerceive = false;
	bool bTalk = false; 
};
