// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/RewindComponent/RewindComponent.h"
#include "RewindComponent.h"

#include "Actors/Projectile/Projectile.h"
#include "Actors/Object/MetalActor.h"

#include "Misc/Utils.h"


// Sets default values for this component's properties
URewindComponent::URewindComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void URewindComponent::BeginPlay()
{
	Super::BeginPlay();

    TransformHistory.Reserve(REWIND_TRANSFORM_COUNT_MAX);
}


// Called every frame
void URewindComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    AActor* Owner = GetOwner();
    if (!Owner) return;

    if (bIsRewinding)
    {
        ApplyRewind();
    }
    else
    {
        StoreRewind();
    }
}

void URewindComponent::StartRewind()
{
    bIsRewinding = true;
    RewindIndex = -1;
}
void URewindComponent::StopRewind()
{
	bIsRewinding = false;
}

void URewindComponent::StoreRewind()
{
    const FTransform Transform = GetOwner()->GetActorTransform();

    const bool bUseBuffer = TransformHistory.Num() == REWIND_TRANSFORM_COUNT_MAX;

    // 가장 최근 Transform 가져오기 (없으면 첫 저장)
    if (!bUseBuffer && TransformHistory.Num() == 0)
    {
        TransformHistory.Add(Transform);
        return;
    }

    const int32 LastIndex = bUseBuffer
        ? (CurrentIndex - 1 + REWIND_TRANSFORM_COUNT_MAX) % REWIND_TRANSFORM_COUNT_MAX
        : TransformHistory.Num() - 1;

    const FTransform& LastTransform = TransformHistory[LastIndex];
    const bool bIsSame = AreTransformsNearlyEqual(Transform, LastTransform);

    if (bIsSame)
    {
        return; // 변화 없으면 저장 안 함
    }

    if (!bUseBuffer)
    {
        TransformHistory.Add(Transform);
        CurrentIndex = (CurrentIndex + 1) % REWIND_TRANSFORM_COUNT_MAX;
    }
    else
    {
        TransformHistory[CurrentIndex] = Transform;
        CurrentIndex = (CurrentIndex + 1) % REWIND_TRANSFORM_COUNT_MAX;
        bBufferFilled = true;
    }
}

void URewindComponent::ApplyRewind()
{
    const int32 HistoryCount = TransformHistory.Num();
    if (HistoryCount == 0) 
    {
        bIsRewinding = false;
        return;
    }

    const int32 Count = bBufferFilled ? REWIND_TRANSFORM_COUNT_MAX : TransformHistory.Num();
    AActor* Owner = GetOwner();
    if (!Owner)
    {
        return;
    }

    // if it starts, init RewindIndex
    if (RewindIndex == -1)
    {
        RewindIndex = (CurrentIndex - 1 + Count) % Count;
    }

    // Apply Current Index's Transform
    const FTransform& Record = TransformHistory[RewindIndex];
    Owner->SetActorTransform(Record);

    // Update Index
    RewindIndex = (RewindIndex - 1 + Count) % Count;

    // check Rewinding is over
    if (RewindIndex  == CurrentIndex)
    {
        RewindIndex = -1;
        bIsRewinding = false;
    }
}

void URewindComponent::SetColorNormal()
{
    AActor* OwnerActor = GetOwner();
    if (OwnerActor->IsA<AProjectile>())
    {
        AProjectile* Projectile = Cast<AProjectile>(OwnerActor);
        Projectile->SetColorNormal();
    }
    else if (OwnerActor->IsA<AMetalActor>())
    {
        AMetalActor* MetalActor = Cast<AMetalActor>(OwnerActor);
        MetalActor->ChangeNomalColor();
    }
}

void URewindComponent::SetColorScanned()
{
    AActor* OwnerActor = GetOwner();
    if (OwnerActor->IsA<AProjectile>())
    {
        AProjectile* Projectile = Cast<AProjectile>(OwnerActor);
        Projectile->SetColorScanned(0.3f);
    }
    else if (OwnerActor->IsA<AMetalActor>())
    {
        AMetalActor* MetalActor = Cast<AMetalActor>(OwnerActor);
        MetalActor->ThisIsMetal(0.3f);
    }
}
