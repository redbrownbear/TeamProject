// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MetalComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TEAMPROJECT_API UMetalComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UMetalComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	bool bIsControlled = false;
public:
	void SetIsControlled(bool bFlag);
	bool GetIsControlled() const;

public:
	void SetColorNormal();
	void SetColorScanned();
};
