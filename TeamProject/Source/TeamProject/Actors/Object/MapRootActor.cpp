// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Object/MapRootActor.h"
#include "Misc/Utils.h"

// Sets default values
AMapRootActor::AMapRootActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

    // Deco
    static ConstructorHelpers::FObjectFinder<UStaticMesh> DecoAsset00 { TEXT("/Script/Engine.StaticMesh'/Game/Assets/MapMesh/FldObj_EnemyBaseBanana_L_A_01_BananaLeaf_00__Mt_Leaf_EnemyBaseBanana_S_A.FldObj_EnemyBaseBanana_L_A_01_BananaLeaf_00__Mt_Leaf_EnemyBaseBanana_S_A'")};
    static ConstructorHelpers::FObjectFinder<UStaticMesh> DecoAsset01 { TEXT("/Script/Engine.StaticMesh'/Game/Assets/MapMesh/FldObj_EnemyBaseBanana_L_A_01_Branch_00__Mt_Wood_EnemyBaseBanana_S_D.FldObj_EnemyBaseBanana_L_A_01_Branch_00__Mt_Wood_EnemyBaseBanana_S_D'")};
    static ConstructorHelpers::FObjectFinder<UStaticMesh> DecoAsset02 { TEXT("/Script/Engine.StaticMesh'/Game/Assets/MapMesh/FldObj_EnemyBaseBanana_L_A_01_cloth_01__Mt_Cloth_EnemyBaseBananaRope_S_A.FldObj_EnemyBaseBanana_L_A_01_cloth_01__Mt_Cloth_EnemyBaseBananaRope_S_A'")};
    static ConstructorHelpers::FObjectFinder<UStaticMesh> DecoAsset03 { TEXT("/Script/Engine.StaticMesh'/Game/Assets/MapMesh/FldObj_EnemyBaseBanana_L_A_01_mud_01__Mt_CmnTex_Sand_Paint_A.FldObj_EnemyBaseBanana_L_A_01_mud_01__Mt_CmnTex_Sand_Paint_A'")};
    static ConstructorHelpers::FObjectFinder<UStaticMesh> DecoAsset04 { TEXT("/Script/Engine.StaticMesh'/Game/Assets/MapMesh/FldObj_EnemyBaseBanana_S_A_01_BananaTree_rope_00__Mt_Cloth_EnemyBaseBanana_S_A.FldObj_EnemyBaseBanana_S_A_01_BananaTree_rope_00__Mt_Cloth_EnemyBaseBanana_S_A'")};
    static ConstructorHelpers::FObjectFinder<UStaticMesh> DecoAsset05 { TEXT("/Script/Engine.StaticMesh'/Game/Assets/MapMesh/FldObj_EnemyBaseBanana_S_A_01_Branch_00__Mt_Wood_EnemyBaseBanana_S_D.FldObj_EnemyBaseBanana_S_A_01_Branch_00__Mt_Wood_EnemyBaseBanana_S_D'")};
    static ConstructorHelpers::FObjectFinder<UStaticMesh> DecoAsset06 { TEXT("/Script/Engine.StaticMesh'/Game/Assets/MapMesh/FldObj_EnemyBaseBanana_S_A_01_leaf_01_1__Mt_Leaf_EnemyBaseBanana_S_A.FldObj_EnemyBaseBanana_S_A_01_leaf_01_1__Mt_Leaf_EnemyBaseBanana_S_A'")};
    static ConstructorHelpers::FObjectFinder<UStaticMesh> DecoAsset07 { TEXT("/Script/Engine.StaticMesh'/Game/Assets/MapMesh/FldObj_EnemyBaseBanana_S_A_01_leaf_06__Mt_CmnTex_Sand_Paint_A.FldObj_EnemyBaseBanana_S_A_01_leaf_06__Mt_CmnTex_Sand_Paint_A'")};
    static ConstructorHelpers::FObjectFinder<UStaticMesh> DecoAsset08 { TEXT("/Script/Engine.StaticMesh'/Game/Assets/MapMesh/FldObj_EnemyLookoutBanana_A_01_damage_00__Mt_Wood_EnemyLookoutBananaSeal_D.FldObj_EnemyLookoutBanana_A_01_damage_00__Mt_Wood_EnemyLookoutBananaSeal_D'")};
    static ConstructorHelpers::FObjectFinder<UStaticMesh> DecoAsset09 { TEXT("/Script/Engine.StaticMesh'/Game/Assets/MapMesh/FldObj_EnemyLookoutBanana_A_01_pole_00_1__Mt_Wood_EnemyBaseBanana_S_D.FldObj_EnemyLookoutBanana_A_01_pole_00_1__Mt_Wood_EnemyBaseBanana_S_D'")};
    static ConstructorHelpers::FObjectFinder<UStaticMesh> DecoAsset10 { TEXT("/Script/Engine.StaticMesh'/Game/Assets/MapMesh/FldObj_EnemyLookoutBanana_A_01_reaf_00__Mt_Leaf_FldObj_EnemyLookoutBanana_A.FldObj_EnemyLookoutBanana_A_01_reaf_00__Mt_Leaf_FldObj_EnemyLookoutBanana_A'")};
    static ConstructorHelpers::FObjectFinder<UStaticMesh> DecoAsset11 { TEXT("/Script/Engine.StaticMesh'/Game/Assets/MapMesh/FldObj_EnemyLookoutBanana_A_01_rope_00_1__Mt_Wood_EnemyLookoutBananaRope_D.FldObj_EnemyLookoutBanana_A_01_rope_00_1__Mt_Wood_EnemyLookoutBananaRope_D'")};

    if (DecoAsset00.Object) { AddDecorationMesh(DecoAsset00.Object); }  
    else { UE_LOG(LogTemp, Error, TEXT("AMapRootActor::AMapRootActor // No StaticMeshAsset")); }
    if (DecoAsset01.Object) { AddDecorationMesh(DecoAsset01.Object); }  
    else { UE_LOG(LogTemp, Error, TEXT("AMapRootActor::AMapRootActor // No StaticMeshAsset")); }
    if (DecoAsset02.Object) { AddDecorationMesh(DecoAsset02.Object); }  
    else { UE_LOG(LogTemp, Error, TEXT("AMapRootActor::AMapRootActor // No StaticMeshAsset")); }
    if (DecoAsset03.Object) { AddDecorationMesh(DecoAsset03.Object); }  
    else { UE_LOG(LogTemp, Error, TEXT("AMapRootActor::AMapRootActor // No StaticMeshAsset")); }
    if (DecoAsset04.Object) { AddDecorationMesh(DecoAsset04.Object); }  
    else { UE_LOG(LogTemp, Error, TEXT("AMapRootActor::AMapRootActor // No StaticMeshAsset")); }
    if (DecoAsset05.Object) { AddDecorationMesh(DecoAsset05.Object); }  
    else { UE_LOG(LogTemp, Error, TEXT("AMapRootActor::AMapRootActor // No StaticMeshAsset")); }
    if (DecoAsset06.Object) { AddDecorationMesh(DecoAsset06.Object); }  
    else { UE_LOG(LogTemp, Error, TEXT("AMapRootActor::AMapRootActor // No StaticMeshAsset")); }
    if (DecoAsset07.Object) { AddDecorationMesh(DecoAsset07.Object); }  
    else { UE_LOG(LogTemp, Error, TEXT("AMapRootActor::AMapRootActor // No StaticMeshAsset")); }
    if (DecoAsset08.Object) { AddDecorationMesh(DecoAsset08.Object); }  
    else { UE_LOG(LogTemp, Error, TEXT("AMapRootActor::AMapRootActor // No StaticMeshAsset")); }
    if (DecoAsset09.Object) { AddDecorationMesh(DecoAsset09.Object); }  
    else { UE_LOG(LogTemp, Error, TEXT("AMapRootActor::AMapRootActor // No StaticMeshAsset")); }
    if (DecoAsset10.Object) { AddDecorationMesh(DecoAsset10.Object); }  
    else { UE_LOG(LogTemp, Error, TEXT("AMapRootActor::AMapRootActor // No StaticMeshAsset")); }
    if (DecoAsset11.Object) { AddDecorationMesh(DecoAsset11.Object); }  
    else { UE_LOG(LogTemp, Error, TEXT("AMapRootActor::AMapRootActor // No StaticMeshAsset")); }


    static ConstructorHelpers::FObjectFinder<UStaticMesh> WalkableAsset00{ TEXT("/Script/Engine.StaticMesh'/Game/Assets/MapMesh/FldObj_EnemyBaseBanana_L_A_01_Stairs_02__Mt_Wood_EnemyBaseBanana_S_F.FldObj_EnemyBaseBanana_L_A_01_Stairs_02__Mt_Wood_EnemyBaseBanana_S_F'") };
    static ConstructorHelpers::FObjectFinder<UStaticMesh> WalkableAsset01{ TEXT("/Script/Engine.StaticMesh'/Game/Assets/MapMesh/FldObj_EnemyBaseBanana_S_A_01_plane_00__Mt_Wood_EnemyBaseBanana_S_F.FldObj_EnemyBaseBanana_S_A_01_plane_00__Mt_Wood_EnemyBaseBanana_S_F'") };
    static ConstructorHelpers::FObjectFinder<UStaticMesh> WalkableAsset02{ TEXT("/Script/Engine.StaticMesh'/Game/Assets/MapMesh/FldObj_EnemyLookoutBanana_A_01_plane_00_1__Mt_Wood_EnemyBaseBanana_S_F.FldObj_EnemyLookoutBanana_A_01_plane_00_1__Mt_Wood_EnemyBaseBanana_S_F'") };

    if (WalkableAsset00.Object) { AddWalkableMesh(WalkableAsset00.Object); } 
    else { UE_LOG(LogTemp, Error, TEXT("AMapRootActor::AMapRootActor // No StaticMeshAsset")); }
    if (WalkableAsset01.Object) { AddWalkableMesh(WalkableAsset01.Object); }
    else { UE_LOG(LogTemp, Error, TEXT("AMapRootActor::AMapRootActor // No StaticMeshAsset")); }
    if (WalkableAsset02.Object) { AddWalkableMesh(WalkableAsset02.Object); }
    else { UE_LOG(LogTemp, Error, TEXT("AMapRootActor::AMapRootActor // No StaticMeshAsset")); }

    // OnlyBlockPlayer
    static ConstructorHelpers::FObjectFinder<UStaticMesh> BlockPlayerOnlyAsset00{ TEXT("/Script/Engine.StaticMesh'/Game/Assets/MapMesh/FldObj_EnemyBaseBanana_L_A_01_BananaTree_02_1__Mt_Wood_EnemyBaseBanana_L_E.FldObj_EnemyBaseBanana_L_A_01_BananaTree_02_1__Mt_Wood_EnemyBaseBanana_L_E'") };
    static ConstructorHelpers::FObjectFinder<UStaticMesh> BlockPlayerOnlyAsset01{ TEXT("/Script/Engine.StaticMesh'/Game/Assets/MapMesh/FldObj_EnemyBaseBanana_L_A_01_polySurface1596__Mt_Wood_EnemyBaseBanana_L_A.FldObj_EnemyBaseBanana_L_A_01_polySurface1596__Mt_Wood_EnemyBaseBanana_L_A'") };
    static ConstructorHelpers::FObjectFinder<UStaticMesh> BlockPlayerOnlyAsset02{ TEXT("/Script/Engine.StaticMesh'/Game/Assets/MapMesh/FldObj_EnemyBaseBanana_L_A_01_polySurface1596__Mt_Wood_EnemyBaseBanana_L_B.FldObj_EnemyBaseBanana_L_A_01_polySurface1596__Mt_Wood_EnemyBaseBanana_L_B'") };
    static ConstructorHelpers::FObjectFinder<UStaticMesh> BlockPlayerOnlyAsset03{ TEXT("/Script/Engine.StaticMesh'/Game/Assets/MapMesh/FldObj_EnemyBaseBanana_S_A_01_BananaTree_02__Mt_Wood_EnemyBaseBanana_S_E.FldObj_EnemyBaseBanana_S_A_01_BananaTree_02__Mt_Wood_EnemyBaseBanana_S_E'") };
    static ConstructorHelpers::FObjectFinder<UStaticMesh> BlockPlayerOnlyAsset04{ TEXT("/Script/Engine.StaticMesh'/Game/Assets/MapMesh/FldObj_EnemyBaseBanana_S_A_01_polySurface1537__Mt_Wood_EnemyBaseBanana_S_A.FldObj_EnemyBaseBanana_S_A_01_polySurface1537__Mt_Wood_EnemyBaseBanana_S_A'") };
    static ConstructorHelpers::FObjectFinder<UStaticMesh> BlockPlayerOnlyAsset05{ TEXT("/Script/Engine.StaticMesh'/Game/Assets/MapMesh/FldObj_EnemyBaseBanana_S_A_01_polySurface1537__Mt_Wood_EnemyBaseBanana_S_B.FldObj_EnemyBaseBanana_S_A_01_polySurface1537__Mt_Wood_EnemyBaseBanana_S_B'") };

    if (BlockPlayerOnlyAsset00.Object) { AddBlockPlayerOnlyMesh(BlockPlayerOnlyAsset00.Object); } 
    else { UE_LOG(LogTemp, Error, TEXT("AMapRootActor::AMapRootActor // No StaticMeshAsset")); }
    if (BlockPlayerOnlyAsset01.Object) { AddBlockPlayerOnlyMesh(BlockPlayerOnlyAsset01.Object); }
    else { UE_LOG(LogTemp, Error, TEXT("AMapRootActor::AMapRootActor // No StaticMeshAsset")); }
    if (BlockPlayerOnlyAsset02.Object) { AddBlockPlayerOnlyMesh(BlockPlayerOnlyAsset02.Object); }
    else { UE_LOG(LogTemp, Error, TEXT("AMapRootActor::AMapRootActor // No StaticMeshAsset")); }
    if (BlockPlayerOnlyAsset03.Object) { AddBlockPlayerOnlyMesh(BlockPlayerOnlyAsset03.Object); }
    else { UE_LOG(LogTemp, Error, TEXT("AMapRootActor::AMapRootActor // No StaticMeshAsset")); }
    if (BlockPlayerOnlyAsset04.Object) { AddBlockPlayerOnlyMesh(BlockPlayerOnlyAsset04.Object); }
    else { UE_LOG(LogTemp, Error, TEXT("AMapRootActor::AMapRootActor // No StaticMeshAsset")); }
    if (BlockPlayerOnlyAsset05.Object) { AddBlockPlayerOnlyMesh(BlockPlayerOnlyAsset05.Object); }
    else { UE_LOG(LogTemp, Error, TEXT("AMapRootActor::AMapRootActor // No StaticMeshAsset")); }

}

// Called when the game starts or when spawned
void AMapRootActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMapRootActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMapRootActor::AddDecorationMesh(UStaticMesh* Mesh)
{
    FString Name = FString::Printf(TEXT("DecorationMesh_%d"), DecorationMeshes.Num());
    UStaticMeshComponent* NewMesh = CreateDefaultSubobject<UStaticMeshComponent>(*Name);

    NewMesh->SetupAttachment(RootComponent);
    NewMesh->SetStaticMesh(Mesh);
    NewMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    NewMesh->SetRelativeScale3D(FVector(DEFAULT_MESH_SCALE));

    DecorationMeshes.Add(NewMesh);
}

void AMapRootActor::AddWalkableMesh(UStaticMesh* Mesh)
{
    FString Name = FString::Printf(TEXT("WalkableMesh_%d"), WalkableMeshes.Num());
    UStaticMeshComponent* NewMesh = CreateDefaultSubobject<UStaticMeshComponent>(*Name);

    NewMesh->SetupAttachment(RootComponent);
    NewMesh->SetStaticMesh(Mesh);
    NewMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    NewMesh->SetCollisionObjectType(ECC_WorldStatic);
    NewMesh->SetCollisionResponseToAllChannels(ECR_Block);
    NewMesh->SetCanEverAffectNavigation(true);
    NewMesh->SetRelativeScale3D(FVector(DEFAULT_MESH_SCALE));
    WalkableMeshes.Add(NewMesh);
}

void AMapRootActor::AddBlockPlayerOnlyMesh(UStaticMesh* Mesh)
{
    FString Name = FString::Printf(TEXT("BlockPlayerOnlyMesh_%d"), BlockPlayerOnlyMeshes.Num());
    UStaticMeshComponent* NewMesh = CreateDefaultSubobject<UStaticMeshComponent>(*Name);

    NewMesh->SetupAttachment(RootComponent);
    NewMesh->SetStaticMesh(Mesh);
    NewMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    NewMesh->SetCollisionObjectType(ECC_WorldStatic);
    NewMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
    NewMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
    NewMesh->SetCanEverAffectNavigation(false);
    NewMesh->SetRelativeScale3D(FVector(DEFAULT_MESH_SCALE));

    BlockPlayerOnlyMeshes.Add(NewMesh);
}