// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Map/MapDataExtractor.h"

#include "Landscape.h"
#include "EngineUtils.h"


// Sets default values
AMapDataExtractor::AMapDataExtractor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMapDataExtractor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMapDataExtractor::ExtractLandscapeData()
{
    MapTiles.Empty();

    for (TActorIterator<ALandscape> It(GetWorld()); It; ++It)
    {
        ALandscape* Landscape = *It;
        if (!Landscape) continue;

        const FVector Origin = Landscape->GetActorLocation();
        const FVector Scale = Landscape->GetActorScale3D();

        int32 ComponentSizeQuads = Landscape->ComponentSizeQuads;  // 보통 63
        int32 NumComponentsX = Landscape->LandscapeComponents.Num();  // 전체 컴포넌트 수
        int32 Resolution = 128; // 높일수록 정밀, 낮출수록 가볍다

        float GridSpacing = 100.f; // 타일 간격 (필요 시 Landscape에서 계산)

        for (int32 X = 0; X < Resolution; ++X)
        {
            for (int32 Y = 0; Y < Resolution; ++Y)
            {
                FVector WorldPos = Origin + FVector(X * GridSpacing, Y * GridSpacing, 0);
                float Height = GetHeightAtLocation(FVector2D(WorldPos.X, WorldPos.Y));

                FMapTileInfo Tile;
                Tile.WorldLocation = FVector(WorldPos.X, WorldPos.Y, Height);
                Tile.Height = Height;
                Tile.bIsWalkable = Height > 10.0f; // 임의 기준

                MapTiles.Add(Tile);
            }
        }

        break;
    }
}

float AMapDataExtractor::GetHeightAtLocation(const FVector2D& XY)
{
    FHitResult Hit;
    FVector Start = FVector(XY.X, XY.Y, 10000.f);
    FVector End = FVector(XY.X, XY.Y, -10000.f);

    FCollisionQueryParams Params;
    Params.bTraceComplex = true;

    if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params))
    {
        if (Hit.GetActor()->IsA<ALandscape>())
        {
            return Hit.ImpactPoint.Z;
        }
    }

    return 0.f; // 지형이 없을 경우
}

