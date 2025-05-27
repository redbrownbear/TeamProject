// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Character/PlayerMovementComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"

UPlayerMovementComponent::UPlayerMovementComponent(const FObjectInitializer& ObjectInitializer)
{


}

TArray<FHitResult> UPlayerMovementComponent::DoCapsuleTraceMultiByObject(const FVector& Start, const FVector& End, bool bShowDebugSphere)
{

    TArray<FHitResult> OutCapsuleTraceHitResult;

    UKismetSystemLibrary::CapsuleTraceMultiForObjects(
        this, Start, End, ClimbCapsuleTraceRadius, ClimbCapsuleTraceHalfHeight,
        ClimbableSurfaceTraceTypes, false, TArray<AActor*>(),
        bShowDebugSphere ? EDrawDebugTrace::ForOneFrame : EDrawDebugTrace::None,
        OutCapsuleTraceHitResult, false


    );


    return OutCapsuleTraceHitResult;
}

void UPlayerMovementComponent::TraceFromEyeHeight(float TraceDistance, float TraceStartOffset)
{
    const FVector ComponentLocation = UpdatedComponent->GetComponentLocation();
    const FVector EyeHeightOffset = UpdatedComponent->GetUpVector() * (CharacterOwner->BaseEyeHeight + TraceStartOffset);
    const FVector Start = UpdatedComponent->GetComponentLocation() + EyeHeightOffset;
    const FVector End = Start + UpdatedComponent->GetForwardVector() * TraceDistance;

    DoLineTraceSingleByObject(Start, End, true);


}

void UPlayerMovementComponent::DoLineTraceSingleByObject(FVector Start, FVector End, bool bShowDebugLine)
{
    TArray<FHitResult> HitResults;
    TArray<AActor*> ActorsToIgnore;
    ActorsToIgnore.Add(GetOwner()); // 자기 자신 무시

    FHitResult OutLineTraceHitResult;
    UKismetSystemLibrary::LineTraceMulti(
        this,
        Start,
        End,
        UEngineTypes::ConvertToTraceType(ECC_Visibility), // Trace 채널
        false,                    // 복잡한 충돌 사용 여부
        ActorsToIgnore,          // 무시할 액터 목록
        EDrawDebugTrace::ForDuration, // 디버그 선 그리기
        HitResults,
        true,                    // 자신 무시
        FLinearColor::Red,       // 선 색
        FLinearColor::Green,     // 히트 선 색
        2.0f                     // 디버그 선 유지 시간
    );

}
