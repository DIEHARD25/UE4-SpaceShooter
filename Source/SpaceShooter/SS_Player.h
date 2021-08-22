// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/AudioComponent.h"
#include "Projectile.h"

#include "SS_Player.generated.h"

UCLASS()
class SPACESHOOTER_API ASS_Player : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ASS_Player();

	USceneComponent * DefaultSceneRoot;
	
	UPROPERTY(EditAnywhere)
		UParticleSystemComponent * ParticleSystem;

	UPROPERTY(EditAnywhere)
		UParticleSystemComponent * Explosion_FX;

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent * ShipMesh;
	
	UPROPERTY(EditAnywhere)
		UCapsuleComponent * CollisionComponent;

	UPROPERTY(EditAnywhere)
		UAudioComponent * DeathExplosionSound;
	
	UPROPERTY(EditAnywhere)
		float Field_Width;
	UPROPERTY(EditAnywhere)
		float Field_Heigth;
	UPROPERTY(BlueprintReadOnly)
		float Max_Health;
	UPROPERTY(BlueprintReadOnly)
		float Current_Health;
	UPROPERTY(BlueprintReadOnly)
		float Max_Armor;
	UPROPERTY(BlueprintReadOnly)
		float Current_Armor;

	void MoveRight(float AxisValue);
	void MoveUp(float AxisValue);

	void UpdateMovement(float DeltaTime);
	void UpdateFiring(float DeltaTime);
	void FireWeapon();
	void StartFiring();
	void StopFiring();
	void CollectablePickup();

	UPROPERTY(EditAnywhere)
		float MaxVelocity;

	bool bIsFiring;
	bool bHit;
	bool bDead;

	float Current_X_Vel;
	float Current_Y_Vel;

	float WeaponFireRate;
	float TimeSinceLastShot;

	UPROPERTY(BlueprintReadOnly)
		float PlayerScore;

	FVector Current_Location;
	FVector New_Location;
	FRotator Current_Rotation;

	UFUNCTION()
		void OnBeginOverlap(AActor * PlayerActor, AActor * OtherActor );

	UPROPERTY(EditAnywhere)
		TSubclassOf<AProjectile> WeaponProjectile_BP;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
