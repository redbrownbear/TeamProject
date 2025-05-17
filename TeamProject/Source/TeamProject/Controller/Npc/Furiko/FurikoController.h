#pragma once

#include "CoreMinimal.h"
#include "Controller/Npc/NpcController.h"
#include "FurikoController.generated.h"

class UFurikoFSMComponent;

UCLASS()
class TEAMPROJECT_API AFurikoController : public ANpcController
{
	GENERATED_BODY()
	
	friend class UFurikoFSMComponent;

public:
	AFurikoController();

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

protected:
	void Hide(); 

protected:
	UPROPERTY()
	TObjectPtr<UFurikoFSMComponent> FurikoFSMComponent;

private:
	bool bPlayHide = false; 
};
