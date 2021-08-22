// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "SS_Player.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	BulletComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlaneComponent"));
	CapsuleCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CollisionComponent"));

	BulletComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	CapsuleCollision->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	ProjectileSpeed = 0.0f;

	bHit = bDestroy = false;
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	CurrentLocation = this->GetActorLocation();

	OnActorBeginOverlap.AddDynamic(this, &AProjectile::OnBeginOverlap);
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	UpdateMovement(DeltaTime);
	if (this->GetActorLocation().X >= 1500)
		bHit = true; // out of bounds
	if (bHit)
		this->Destroy();

}

void AProjectile::OnBeginOverlap(AActor* ProjectileActor, AActor* OtherActor)
{
	if (OtherActor)
	{
		// finding out who fired this projectile
		ASS_Player* OwningPlayer = Cast<ASS_Player>(this->GetOwner());

		if (OtherActor->ActorHasTag("Bounds") || (this->ActorHasTag("EnemyProjectile") && OtherActor->ActorHasTag("Player")))
			bHit = true;

		if (OtherActor->ActorHasTag("EnemyShip") && OwningPlayer)
		{
			OwningPlayer->PlayerScore += 50.0f;
			bHit = true;
		}

		if (OtherActor->ActorHasTag("Asteroid") && OwningPlayer)
		{
			OwningPlayer->PlayerScore += 10.0f;
			bHit = true;
		}		
	}
}

void AProjectile::UpdateMovement(float DeltaTime)
{
	FVector NewLocation = FVector(CurrentLocation.X + (ProjectileSpeed * DeltaTime),
		CurrentLocation.Y,
		CurrentLocation.Z);
	this->SetActorLocation(NewLocation);
	CurrentLocation = NewLocation;
}
