// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/AudioComponent.h"
#include "SS_Pickup.h"

#include "Projectile.h"
#include "SS_Enemy.generated.h"

UCLASS()
class SPACESHOOTER_API ASS_Enemy : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ASS_Enemy();

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditAnywhere)
		UCapsuleComponent* CollisionComponent;

	UPROPERTY(EditAnywhere)
		UParticleSystemComponent* Explosion_FX;

	UPROPERTY(EditAnywhere)
		UAudioComponent* ExplosionSound;

	UPROPERTY(EditAnywhere)
		TSubclassOf<AProjectile> EnemyProjectile_BP;

	UWorld* ThisWorld;

	UPROPERTY(EditAnywhere)
		TSubclassOf<ASS_Pickup> Pickup_Can;

	UPROPERTY(EditAnywhere)
		FVector Current_Velocity;

	void UpdateMovement(float DeltaTime);
	void UpdateFiring(float DeltaTime);

	FVector Current_Location;
	FRotator Current_Rotation;

	float TotalTime;
	float TimeSinceLastShot;
	float RandomStart;
	float fDestroyTimer;
	float fBurstDelay;

	bool bHit;
	bool bDead;

	UFUNCTION()
		void OnBeginOverlap(AActor* EnemyActor, AActor* OtherActor);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
