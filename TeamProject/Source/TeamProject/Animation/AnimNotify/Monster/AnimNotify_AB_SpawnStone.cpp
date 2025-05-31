// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify/Monster/AnimNotify_AB_SpawnStone.h"
#include "Actors/Monster/CharacterMonster.h"
#include "Components/FSMComponent/Monster/AssasinBossFSMComponent.h"
#include "Actors/Projectile/KogaStone.h"

void UAnimNotify_AB_SpawnStone::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	UE_LOG(LogTemp, Warning, TEXT("UAnimNotify_AB_SpawnStone::Notify"));

	if (ACharacterMonster* CharacterMonster = Cast<ACharacterMonster>(MeshComp->GetOwner()))
	{
		UWorld* World = MeshComp->GetWorld();


		if (UAssasinBossFSMComponent* FSMComponent = Cast< UAssasinBossFSMComponent>(CharacterMonster->GetFSMComponent()))
		{
			EAssasinBossPhase ePhase = FSMComponent->GetPhase();

			switch (ePhase)
			{
			case EAssasinBossPhase::PHASE_1:
			{
				AKogaStone* KogaStone = World->SpawnActorDeferred<AKogaStone>(AKogaStone::StaticClass(),
					FTransform::Identity, nullptr, CharacterMonster, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);


				FTransform NewTransform;
				KogaStone->SetData(ProjectileName::Monster_AB_KogaStone, CollisionProfileName::ToPlayer);

				const FVector Location = CharacterMonster->GetActorLocation();
				const FVector TargetLocation = Location + FVector(0.0, 0.0, 1.0) * KOGASTONE_ORBIT_RADIUS;

				NewTransform.SetLocation(TargetLocation);

				KogaStone->FinishSpawning(NewTransform);
			}
				
				break;
			case EAssasinBossPhase::PHASE_2:
			{
				AKogaStone* Projectile = World->SpawnActorDeferred<AKogaStone>(AKogaStone::StaticClass(),
					FTransform::Identity, nullptr, CharacterMonster, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);


				FTransform NewTransform;
				Projectile->SetData(ProjectileName::Monster_AB_KogaStone, CollisionProfileName::ToPlayer);

				const FVector MonsterLocation = CharacterMonster->GetActorLocation();
				FVector Location = MonsterLocation + CharacterMonster->GetActorRightVector() * KOGASTONE_ORBIT_RADIUS;



				NewTransform.SetLocation(Location);

				Projectile->FinishSpawning(NewTransform);
				//////////////////////////////////////////
				Projectile = World->SpawnActorDeferred<AKogaStone>(AKogaStone::StaticClass(),
					FTransform::Identity, nullptr, CharacterMonster, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);


				Projectile->SetData(ProjectileName::Monster_AB_KogaStone, CollisionProfileName::ToPlayer);
				Location = MonsterLocation - CharacterMonster->GetActorRightVector() * KOGASTONE_ORBIT_RADIUS;
				NewTransform.SetLocation(Location);

				Projectile->FinishSpawning(NewTransform);
			}
				break;
			case EAssasinBossPhase::PHASE_3:
			{
				AKogaStone* KogaStone = World->SpawnActorDeferred<AKogaStone>(AKogaStone::StaticClass(),
					FTransform::Identity, nullptr, CharacterMonster, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);


				FTransform NewTransform;
				KogaStone->SetData(ProjectileName::Monster_AB_KogaStoneBig, CollisionProfileName::ToPlayer);

				const FVector Location = CharacterMonster->GetActorLocation();
				const FVector TargetLocation = Location + FVector(0.0, 0.0, 1.0) * KOGASTONE_ORBIT_RADIUS;

				NewTransform.SetLocation(TargetLocation);

				KogaStone->FinishSpawning(NewTransform);
			}
				break;
			case EAssasinBossPhase::END:
				break;
			default:
				break;
			}
		}
	}



}
