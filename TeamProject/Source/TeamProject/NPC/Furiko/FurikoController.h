#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "FurikoController.generated.h"

class UFurikoFSMComponent;

UCLASS()
class TEAMPROJECT_API AFurikoController : public AAIController
{
	GENERATED_BODY()
	
public:
	AFurikoController();

protected:
	UPROPERTY()
	TObjectPtr<UFurikoFSMComponent> NpcFSMComponent;

};
