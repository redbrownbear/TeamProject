// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Object/TorchStand.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "Components/SphereComponent.h"
#include "Actors/TriggerBox/TorchTriggerBox.h"
#include "Actors/Character/PlayerCharacter.h"
#include "Misc/Utils.h"


ATorchStand::ATorchStand()
{
	PrimaryActorTick.bCanEverTick = false;

    StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
    RootComponent = StaticMeshComponent;

    static ConstructorHelpers::FObjectFinder<UStaticMesh> Asset0
    { TEXT("/Script/Engine.StaticMesh'/Game/Resources/Map/AssasinHideout/FldObj_TorchStand_A_01_Root__Mt_TorchStand_A.FldObj_TorchStand_A_01_Root__Mt_TorchStand_A'") };
    if (Asset0.Object)
    {
        StaticMeshComponent->SetStaticMesh(Asset0.Object);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("ATorchStand::ATorchStand // No TorchStand StaticMeshAsset"));
    }




    NukiStaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("NukiStaticMeshComponent"));

    static ConstructorHelpers::FObjectFinder<UStaticMesh> Asset1
    { TEXT("/Script/Engine.StaticMesh'/Game/Resources/Map/AssasinHideout/FldObj_TorchStand_A_01_Root__Mt_TorchStand_A_Nuki.FldObj_TorchStand_A_01_Root__Mt_TorchStand_A_Nuki'") };
    if (Asset1.Object)
    {
        NukiStaticMeshComponent->SetStaticMesh(Asset1.Object);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("ATorchStand::ATorchStand // No TorchStandNuki StaticMeshAsset"));
    }
    NukiStaticMeshComponent->SetupAttachment(StaticMeshComponent);
    NukiStaticMeshComponent->SetRelativeLocation(FVector(0.f, 0.f, -1.f));

    NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));

    static ConstructorHelpers::FObjectFinder<UNiagaraSystem> FireAsset
    { TEXT("/Script/Niagara.NiagaraSystem'/Game/Vefects/Free_Fire/Shared/Particles/NS_Fire_Small.NS_Fire_Small'") };

    if (FireAsset.Object)
    {
        NiagaraComponent->SetAsset(FireAsset.Object);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("ACampFire::ACampFire // No Fire NiagaraAsset"));
    }

    NiagaraComponent->SetupAttachment(RootComponent);
    NiagaraComponent->SetRelativeLocation(FVector(0.f, 0.f, 50.f));



    TriggerColliderComponent = CreateDefaultSubobject<USphereComponent>(TEXT("TriggerColliderComponent"));
    TriggerColliderComponent->bHiddenInGame = COLLISION_HIDDEN_IN_GAME;
    TriggerColliderComponent->SetSphereRadius(32.f);
    TriggerColliderComponent->SetupAttachment(RootComponent);
    TriggerColliderComponent->SetRelativeLocation(FVector(0.f, 0.f, 70.f));
    TriggerColliderComponent->OnComponentBeginOverlap.AddDynamic(this, &ATorchStand::OnBeginOverlap);
}

// Called when the game starts or when spawned
void ATorchStand::BeginPlay()
{
    Super::BeginPlay();
    
    TriggerColliderComponent->SetCollisionProfileName(TEXT("Trigger"));

    if (NiagaraComponent->IsActive())
    {
        bTorchOnFire = true;
    }
}

void ATorchStand::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (AProjectile_Arrow* Arrow = Cast<AProjectile_Arrow>(OtherActor))
    {
        UE_LOG(LogTemp, Warning, TEXT("[Torch: %s] Overlap 시작! 화살: %s (이름: %s, bTorchOnFire: %s)"),
            *GetName(), *Arrow->GetName(), *Arrow->GetProjectileName().ToString(), bTorchOnFire ? TEXT("true") : TEXT("false"));

        if (!bTorchOnFire && ProjectileName::Player_FireArrow != Arrow->GetProjectileName()) return; // 횃대에 불이 붙어있지 않을 때

        APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
        if (ProjectileName::Player_Arrow == Arrow->GetProjectileName())
        {
            PlayerCharacter->SetArrowFire(true);
            Arrow->SetData(TEXT("Player_FireArrow"), TEXT("ToMonster"));

            UE_LOG(LogTemp, Warning, TEXT("SetData 이후 ProjectileName: %s"), *Arrow->GetProjectileName().ToString());
            //UE_LOG(LogTemp, Log, TEXT("Arrow is Fired!"));
        }
        else if (ProjectileName::Player_FireArrow == Arrow->GetProjectileName())
        {
            if (PlayerCharacter->GetIsFire()) //화살에 불이 붙었을 때
            {
                if (!bTorchOnFire) // 횃대에 불이 붙어있지 않을 때
                {
                    SetTorchStandFire(PlayerCharacter->GetIsFire());
                }
            }
        }
    }

    else if (APlayerCharacter* Player_C = Cast<APlayerCharacter>(OtherActor))
    {
        if (Player_C)
        {
            {
                UWeaponManagerComponent* WeaponManager = Player_C->GetWeaponManagerComponent();
                if (Player_C->GetWeaponManagerComponent()->GetEquipState() != EEquip_State::Bow)
                {
                    return;
                }
                if (WeaponManager->GetCanShot())
                {
                    Player_C->SetArrowFire(true);
                }
            }
        }
    }


    
  /*  else
    {
        AProjectile_Arrow* Projectile = Cast<AProjectile_Arrow>(OtherActor);
        if (!Projectile)
        {
            return;
        }
        Projectile->SetData(TEXT("Player_FireArrow"), TEXT("ToMonster"));

    }*/


}

// Called every frame
void ATorchStand::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATorchStand::SetTorchStandFire(bool _bool)
{
    bTorchOnFire = _bool;
    if (_bool)
    {
        SetNiagaraVisibility(true);
    }
    else
    {
        SetNiagaraVisibility(false);
    }
}

void ATorchStand::SetNiagaraVisibility(bool bFlag)
{
    if (!NiagaraComponent)
    {
        return;
    }

    if (bFlag)
    {
        NiagaraComponent->Activate();
    }
    else
    {
        NiagaraComponent->Deactivate();
    }
}