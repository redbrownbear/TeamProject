// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify/Monster/AnimNotify_HinoxWalkShakeCamera.h"
#include "Actors/Monster/CharacterMonster.h"
#include "Shakes/DefaultCameraShakeBase.h"
#include "Camera/CameraModifier_CameraShake.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"

	/** Static.  Plays an in-world camera shake that affects all nearby players, with radial distance-based attenuation.
	 * @param InWorld - World context.
	 * @param Shake - Camera shake asset to use.
	 * @param Epicenter - Location to place the effect in world space
	 * @param InnerRadius - Cameras inside this radius get the full intensity shake.
	 * @param OuterRadius - Cameras outside this radius are not affected.
	 * @param Falloff - Exponent that describes the shake intensity falloff curve between InnerRadius and OuterRadius. 1.0 is linear.
	 * @param bOrientShakeTowardsEpicenter - Changes the rotation of shake to point towards epicenter instead of forward. Useful for things like directional hits.
	 */

// void APlayerCameraManager::PlayWorldCameraShake(UWorld* InWorld, TSubclassOf<class UCameraShakeBase> Shake, FVector Epicenter, float InnerRadius, float OuterRadius, float Falloff, bool bOrientShakeTowardsEpicenter )

void UAnimNotify_HinoxWalkShakeCamera::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (ACharacterMonster* Monster = Cast<ACharacterMonster>(MeshComp->GetOwner()))
	{
		if (UDefaultCameraShakeBase* CameraShakeBase = Monster->GetDefaultCameraShakeBase())
		{
			if (UWorld* World = Monster->GetWorld())
			{
				FVector Location = Monster->GetActorLocation();
				APlayerCameraManager::PlayWorldCameraShake(World, CameraShakeBase->GetClass(), Location, 1000.f, 1500.f, 1.0);
			}
		}
	}
}
