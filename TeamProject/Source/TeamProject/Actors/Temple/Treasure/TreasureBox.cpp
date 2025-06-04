#include "Actors/Temple/Treasure/TreasureBox.h"
#include "Components/BoxComponent.h"
#include "Actors/Effect/ParticleEffect.h"

#include "Actors/Character/PlayerCharacter.h"
#include "GameFramework/PC_InGame.h"

#include "UI/HUD/MainHUD.h"
#include "Data/ItemDataRow.h"
#include "SubSystem/UI/UIManager.h"

// Sets default values
ATreasureBox::ATreasureBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComponent"));
	SetRootComponent(CollisionComponent);
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	SkeletalMeshComponent->SetupAttachment(RootComponent);
	SkeletalMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

}

// Called when the game starts or when spawned
void ATreasureBox::BeginPlay()
{
	Super::BeginPlay();
	
	CollisionComponent->SetCollisionObjectType(ECC_WorldDynamic);
	CollisionComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	CollisionComponent->SetGenerateOverlapEvents(true);

	SkeletalMeshComponent->SetCollisionObjectType(ECC_WorldStatic);
	SkeletalMeshComponent->SetCollisionResponseToAllChannels(ECR_Block);

	MaterialInterface = SkeletalMeshComponent->GetMaterial(0);
	DynamicMaterialInstance = UMaterialInstanceDynamic::Create(MaterialInterface, this);
	SkeletalMeshComponent->SetMaterial(0, DynamicMaterialInstance);

	if (CollisionComponent)
	{
		CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ATreasureBox::OnBeginOverlapWithPlayer);
		CollisionComponent->OnComponentEndOverlap.AddDynamic(this, &ATreasureBox::OnEndOverlapWithPlayer);
	}

	bCanTakeItem = true;
}

void ATreasureBox::OpenTBox()
{
	if (!bCanTakeItem && !bCanOpenBox)
	{
		UE_LOG(LogTemp, Log, (TEXT("This Box is empty")));
	}

	ShowItemByRowName(ItemRowHandle.RowName);

	GetTreasure();
}

void ATreasureBox::CloseUI()
{
	PopupItemUI->HideUI(UPopupGetItem::StaticClass());
}

void ATreasureBox::OnBeginOverlapWithPlayer(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor))
	{
		if (APC_InGame* PC = Cast<APC_InGame>(Player->GetController()))
		{
			PC->TreasureBox = this;
			bPlayerInRange = true;

			if (AMainHUD* HUD = Cast<AMainHUD>(PC->GetHUD()))
			{
				HUD->ShowInteractWidget(bPlayerInRange);
			}
		}
	}

	if (bCanTakeItem)
	{
		bCanOpenBox = true;
		GetParticleEffect();
	}
}

void ATreasureBox::OnEndOverlapWithPlayer(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	bCanOpenBox = false;

	if (APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor))
	{
		if (APC_InGame* PC = Cast<APC_InGame>(Player->GetController()))
		{
			PC->TreasureBox = this;
			bPlayerInRange = false;

			if (AMainHUD* HUD = Cast<AMainHUD>(PC->GetHUD()))
			{
				HUD->ShowInteractWidget(bPlayerInRange);
			}
		}
	}
}

void ATreasureBox::GetTreasure()
{
	APC_InGame* PC_InGame = Cast<APC_InGame>(UGameplayStatics::GetPlayerController(this, 0));
	if (PC_InGame)
	{
		if (ItemDataPtr && bCanTakeItem)
		{
			// open Item UI
			ShowItemPopup(ItemRowHandle.RowName);

			// Add Item to Inventory
			AddItemInventory();

			DynamicMaterialInstance->SetScalarParameterValue("Color", 1.f); 
			
			EmptyBox();
		}

		PC_InGame->ChangeInputContext(EInputContext::IC_InGame);
	}
}


void ATreasureBox::GetParticleEffect()
{
	if (!ParticleEffectTableRowHandle.IsNull())
	{
		UWorld* World = GetWorld();
		if (!World) return;

		FTransform SpawnTransform;
		SpawnTransform.SetLocation(GetActorLocation());
		SpawnTransform.SetRotation(FRotator::ZeroRotator.Quaternion());

		AParticleEffect* Effect = World->SpawnActorDeferred<AParticleEffect>(AParticleEffect::StaticClass(),
			FTransform::Identity, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		if (Effect)
		{
			Effect->SetData(ParticleEffectTableRowHandle);
			Effect->FinishSpawning(SpawnTransform);
		}
	}
}

void ATreasureBox::ShowItemByRowName(FName RowName)
{
	/*if (!ItemDataTable)
	{
		UE_LOG(LogTemp, Warning, TEXT("ItemDataTable is null"));
		return;
	}*/

	//ItemDataPtr = ItemDataTable->FindRow<FItemData>(RowName, TEXT("Find Item Row"));

	ItemDataPtr = ItemRowHandle.GetRow<FItemData>(TEXT("Find Item Row"));

	if (!ItemDataPtr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Item Row not found: %s"), *RowName.ToString());
	}
}

void ATreasureBox::ShowItemPopup(FName ItemRowName)
{
	UUIManager* UIManager = GetGameInstance()->GetSubsystem<UUIManager>();
	if (UIManager)
	{
		UIManager->ShowUI(UPopupGetItem::StaticClass());
	}

	PopupItemUI = UIManager->FindUI<UPopupGetItem>();
	if (PopupItemUI)
	{
		PopupItemUI->ShowData(*ItemDataPtr);
	}
}

void ATreasureBox::AddItemInventory()
{
	UPlayerManager* PlayerManager = GetGameInstance()->GetSubsystem<UPlayerManager>();
	if (PlayerManager)
	{
		PlayerManager->SetInvenData(*ItemDataPtr);
	}
}
