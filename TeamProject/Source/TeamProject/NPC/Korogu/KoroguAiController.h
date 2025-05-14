#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "KoroguAiController.generated.h"

class UKoroguFSMComponent;

UCLASS()
class TEAMPROJECT_API AKoroguAiController : public AAIController
{
	GENERATED_BODY()
	
public:
	AKoroguAiController();

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

};
