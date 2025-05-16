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

protected:
	virtual void BeginPlay() override;

protected:
	void SetFocus(); // 시선 처리 함수
};
