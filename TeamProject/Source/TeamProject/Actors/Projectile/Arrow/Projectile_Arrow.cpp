// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Projectile/Arrow/Projectile_Arrow.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/ShapeComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Misc/Utils.h"


AProjectile_Arrow::AProjectile_Arrow()
{
	
	ProjectileMovementComponent->ProjectileGravityScale = 0.1f;
	ProjectileMovementComponent->InitialSpeed = Arrow_Speed;
	ProjectileMovementComponent->MaxSpeed = Arrow_Speed;
	ProjectileMovementComponent->bShouldBounce = false;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	StaticMeshComponent->SetRelativeScale3D(FVector(DEFAULT_MESH_SCALE*10, DEFAULT_MESH_SCALE * 10, DEFAULT_MESH_SCALE * 10));
	ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMesh(TEXT("/Script/Engine.StaticMesh'/Game/Resources/Player/Arrow/Weapon_Arrow_020_Mesh.Weapon_Arrow_020_Mesh'"));
	if (StaticMesh.Object)
	{
		StaticMeshComponent->SetStaticMesh(StaticMesh.Object);
	}

	CollisionComponent->SetRelativeScale3D(FVector(0.1f, 0.1f, 0.1f));
	//CollisionComponent->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel5);
	CollisionComponent->SetCollisionProfileName(CollisionProfileName::ToMonster);

	GetProjectileName();

}

void AProjectile_Arrow::BeginPlay()
{
	Super::BeginPlay();






}

void AProjectile_Arrow::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	Destroy();
}

//void AProjectile_Arrow::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
//{
//	Super::OnHit(HitComponent, OtherActor,OtherComp,NormalImpulse,Hit);
//}

void AProjectile_Arrow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector Velocity = ProjectileMovementComponent->Velocity;
	if (!Velocity.IsNearlyZero())
	{
		FRotator NewRotation = Velocity.Rotation();
		StaticMeshComponent->SetWorldRotation(NewRotation);
	}
}

FName AProjectile_Arrow::GetProjectileName()
{
	return ProjectileName::Player_Arrow;
}




