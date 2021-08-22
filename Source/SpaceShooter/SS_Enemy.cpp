// Fill out your copyright notice in the Description page of Project Settings.


#include "SS_Enemy.h"

// Sets default values
ASS_Enemy::ASS_Enemy()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	// mesh & collision
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	CollisionComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CollisionComponent"));
	// particles
	Explosion_FX = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ExplosionFX"));
	// audio
	ExplosionSound = CreateDefaultSubobject<UAudioComponent>(TEXT("ExplosionSound"));
	// now add them to rootComponent
	StaticMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	CollisionComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	Explosion_FX->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	ExplosionSound->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}

// Called when the game starts or when spawned
void ASS_Enemy::BeginPlay()
{
	Super::BeginPlay();
	
	// init starting state
	bHit = bDead = false;
	TotalTime = TimeSinceLastShot = 0.0f;
	fDestroyTimer = 1.0f;

	Explosion_FX->Deactivate();
	ExplosionSound->Deactivate();

	ThisWorld = GetWorld();

	RandomStart = FMath::Rand();

	OnActorBeginOverlap.AddDynamic(this, &ASS_Enemy::OnBeginOverlap);
}

// Called every frame
void ASS_Enemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateMovement(DeltaTime);
	UpdateFiring(DeltaTime);

	if (bDead) this->Destroy();
	if (bHit)
	{
		StaticMesh->SetVisibility(false);
		this->SetActorEnableCollision(false);
		Explosion_FX->Activate();
		ExplosionSound->Activate();

		fDestroyTimer -= DeltaTime;
	}
	if (fDestroyTimer <= 0) bDead = true;
}

// Called to bind functionality to input
void ASS_Enemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ASS_Enemy::UpdateMovement(float DeltaTime)
{
	Current_Location = this->GetActorLocation();
	Current_Rotation = this->GetActorRotation();

	Current_Location.Y += FMath::Sin(TotalTime + RandomStart);

	this->SetActorLocation(Current_Location - (Current_Velocity * DeltaTime));

	TotalTime += DeltaTime;
}

void ASS_Enemy::UpdateFiring(float DeltaTime)
{
	TimeSinceLastShot += DeltaTime;
	fBurstDelay += DeltaTime;
	if (TimeSinceLastShot >= 1.0f && !bHit && fBurstDelay >= 0.05f)
	{
		FActorSpawnParameters Params = {};
		Params.Owner = this;

		ThisWorld->SpawnActor(EnemyProjectile_BP, &Current_Location, &Current_Rotation, Params);
		// resetting
		fBurstDelay = 0.0f;
		TimeSinceLastShot = 0.0f;		
	}
}

void ASS_Enemy::OnBeginOverlap(AActor* EnemyActor, AActor* OtherActor)
{
	if (OtherActor->ActorHasTag("Bounds"))
		bDead = true;
	if (OtherActor->ActorHasTag("Asteroid") || OtherActor->ActorHasTag("Player"))
		bHit = true;
	if (OtherActor->ActorHasTag("Projectile"))
	{
		if (ThisWorld)
		{
			bHit = true;
			// TODO: spawn pickup
			if (FMath::RandRange(0, 10) > 7)
			{
				FVector CurrentSpawnLocation = this->GetActorLocation();
				FRotator CurrrntSpawnRotation = this->GetActorRotation();
				FActorSpawnParameters Params = {};

				ThisWorld->SpawnActor(Pickup_Can, &CurrentSpawnLocation, &CurrrntSpawnRotation, Params);
			}
		}
	}
}
