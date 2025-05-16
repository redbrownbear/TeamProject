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

public:
	void Stroll(); // 돌아다니기
	void MoveToStrollPoint(); // Stroll Path
	void Interact(); // 접근(상호작용)
	void Talk(); // 대화 함수

protected:
	UPROPERTY()
	TObjectPtr<UNpcFSMComponent> NpcFSMComponent;

public:
	bool bPerceive = false;
	bool bTalk = false; // 대화 
};
