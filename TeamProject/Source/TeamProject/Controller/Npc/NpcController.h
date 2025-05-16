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
	void Stroll(); // ���ƴٴϱ�
	void MoveToStrollPoint(); // Stroll Path
	void Interact(); // ����(��ȣ�ۿ�)
	void Talk(); // ��ȭ �Լ�

protected:
	UPROPERTY()
	TObjectPtr<UNpcFSMComponent> NpcFSMComponent;

public:
	bool bPerceive = false;
	bool bTalk = false; // ��ȭ 
};
