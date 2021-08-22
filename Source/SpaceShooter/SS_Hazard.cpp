// Fill out your copyright notice in the Description page of Project Settings.


#include "SS_Hazard.h"

// Sets default values
ASS_Hazard::ASS_Hazard()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	// mesh & collision
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	CollisionComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CollisionComponent"));
	// particles
	AsteroidExplosion_FX = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("AsteroidExplosion_FX"));
	// audio
	AsteroidExplosionSound = CreateDefaultSubobject<UAudioComponent>(TEXT("AsteroidExplosionSound"));
	// now add them to rootComponent
	StaticMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	CollisionComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	AsteroidExplosion_FX->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	AsteroidExplosionSound->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	Spawn_X_Vel = -500.0f;
	Spawn_Y_Vel = 0.0f;

	SelfDestructTimer = 1.0f;

	bHit = false;

}

// Called when the game starts or when spawned
void ASS_Hazard::BeginPlay()
{
	Super::BeginPlay();

	InitialRotation = 0.0f;
	Initial_X_Loc = 1500.0f;
	Initial_Y_Loc = FMath::RandRange(-350.0f, 350.0f);
	RandSize = FMath::RandRange(3, 8);
	this->SetActorScale3D(FVector(RandSize, RandSize, RandSize));

	AsteroidExplosion_FX->Deactivate();

	AsteroidExplosionSound->Deactivate();
	// allow the sound to play till end
	AsteroidExplosionSound->bStopWhenOwnerDestroyed = false;

	OnActorBeginOverlap.AddDynamic(this, &ASS_Hazard::OnBeginOverlap);
}

// Called every frame
void ASS_Hazard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	check(this != nullptr);

	InitialRotation += DeltaTime;

	Initial_X_Loc += DeltaTime * Spawn_X_Vel;
	Initial_Y_Loc += DeltaTime * Spawn_Y_Vel;

	this->SetActorRotation(FRotator(InitialRotation * 100.0f, InitialRotation * 50.0f, 0.0f));
	this->SetActorLocation(FVector(Initial_X_Loc, Initial_Y_Loc, 0.0f));

	if (SelfDestructTimer <= 0.0f)
		this->Destroy();

	if (bHit)
	{
		bHit = false;
		bStartDestroyTimer = true;
		AsteroidExplosion_FX->Activate();
		AsteroidExplosion_FX->SetWorldLocation(this->GetActorLocation());
		AsteroidExplosion_FX->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));

		AsteroidExplosionSound->Activate();
		AsteroidExplosionSound->Play();

		this->StaticMesh->SetVisibility(false);
		this->SetActorEnableCollision(false);

		if (this->GetActorScale3D().X > 6.0f || this->GetActorScale3D().Y > 6.0f || this->GetActorScale3D().Z > 6.0f)
		{
			SpawnChildren(FMath::RandRange(1,4));
		}
	}
	if (bStartDestroyTimer)
		SelfDestructTimer -= DeltaTime;
}

void ASS_Hazard::SpawnChildren(int32 Asteroid_Children)
{
	FActorSpawnParameters Params = {};
	Params.Owner = this;

	for (int i = 0; i < Asteroid_Children; ++i)
	{
		if (ChildSpawn)
		{
			ASS_Hazard* NewAsteroid = Cast < ASS_Hazard>(GetWorld()->SpawnActor(ChildSpawn, 
																				new FVector(this->GetActorLocation()), 
																				new FRotator(this->GetActorRotation()), 
																				Params));
			NewAsteroid->Initial_X_Loc = this->GetActorLocation().X;
			NewAsteroid->Initial_Y_Loc = this->GetActorLocation().Y;
			NewAsteroid->SetHazardVelocity(FVector(FMath::RandRange(-250,100),
												   FMath::RandRange(-50, 50),
												   0.0f));
			float RandScale = FMath::RandRange(2, 5);
			NewAsteroid->SetActorScale3D(FVector(RandScale, RandScale, RandScale));
			NewAsteroid->SetActorEnableCollision(true);
			NewAsteroid->StaticMesh->SetVisibility(true);
		}

	} // loop
}

void ASS_Hazard::SetHazardVelocity(FVector NewVelocity)
{
	Spawn_X_Vel = NewVelocity.X;
	Spawn_Y_Vel = NewVelocity.Y;
}

void ASS_Hazard::OnBeginOverlap(AActor* AsteroidActor, AActor* OtherActor)
{
	if (OtherActor->ActorHasTag("Bounds"))
		SelfDestructTimer = 0.0f;

	if (OtherActor->ActorHasTag("Projectile") || OtherActor->ActorHasTag("Player"))
	{
		bHit = true;
	}
}
