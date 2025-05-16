#pragma once

#include "CoreMinimal.h"
#include "Controller/Npc/NpcController.h"
#include "FurikoController.generated.h"

UCLASS()
class TEAMPROJECT_API AFurikoController : public ANpcController
{
	GENERATED_BODY()
	
public:
	AFurikoController();

	virtual void BeginPlay() override;

};
