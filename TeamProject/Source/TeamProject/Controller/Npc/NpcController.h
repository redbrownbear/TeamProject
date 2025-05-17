#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "NpcController.generated.h"

class UNpcFSMComponent;
class AStrollPath;

UCLASS()
class TEAMPROJECT_API ANpcController : public AAIController
{
	GENERATED_BODY()

public:
	ANpcController();

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY()
	TObjectPtr<UNpcFSMComponent> NpcFSMComponent;

public:
	bool bPerceive = false;
	bool bTalk = false; 
};
