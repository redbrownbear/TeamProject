// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify/Monster/AnimNotify_HinoxGrabStone.h"
#include "Actors/Monster/CharacterMonster.h"
#include "Actors/Object/ThrownObject.h"

void UAnimNotify_HinoxGrabStone::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	static UDataTable* ThrownObjectDataTable = LoadObject<UDataTable>(nullptr, TEXT("/Script/Engine.DataTable'/Game/Data/ItemData/DT_ThrownObject.DT_ThrownObject'"));

	if (!ThrownObjectDataTable)
	{
		ThrownObjectDataTable = LoadObject<UDataTable>(nullptr, TEXT("/Script/Engine.DataTable'/Game/Data/ItemData/DT_ThrownObject.DT_ThrownObject'"));
		check(ThrownObjectDataTable);
	}


	if (ACharacterMonster* Monster = Cast<ACharacterMonster>(MeshComp->GetOwner()))
	{
		UWorld* World = MeshComp->GetWorld();

		AThrownObject* ThrownObject = World->SpawnActorDeferred<AThrownObject>(AThrownObject::StaticClass(),
			FTransform::Identity, nullptr, Monster, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		FDataTableRowHandle DataTableRowHandle;
		DataTableRowHandle.DataTable = ThrownObjectDataTable;
		DataTableRowHandle.RowName = ThrownObject::HinoxRock;

		ThrownObject->SetDataWithHandle(DataTableRowHandle);
		ThrownObject->AttachToMonster(Monster, Monster_SocketName::Weapon_R);
		ThrownObject->FinishSpawning(FTransform::Identity);
		Monster->SetThrownObject(ThrownObject);
	}
}
