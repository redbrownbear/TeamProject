#pragma once

#include "CoreMinimal.h"
#include "Actors/Controller/Npc/NpcController.h"
#include "FurikoController.generated.h"

UCLASS()
class TEAMPROJECT_API AFurikoController : public ANpcController
{
	GENERATED_BODY()

public:
	AFurikoController();

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

};
