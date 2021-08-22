// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/AudioComponent.h"
#include "Particles/ParticleSystemComponent.h"

#include "SS_Hazard.generated.h"

UCLASS()
class SPACESHOOTER_API ASS_Hazard : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASS_Hazard();

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditAnywhere)
		UCapsuleComponent* CollisionComponent;

	UPROPERTY(EditAnywhere)
		UAudioComponent* AsteroidExplosionSound;

	UPROPERTY(EditAnywhere)
		UParticleSystemComponent* AsteroidExplosion_FX;

	UPROPERTY(EditAnywhere)
		TSubclassOf<ASS_Hazard> ChildSpawn;

	UFUNCTION()
		void OnBeginOverlap(AActor* AsteroidActor, AActor* OtherActor);

	float InitialRotation;
	float Initial_X_Loc;
	float Initial_Y_Loc;
	float RandSize;
	float SelfDestructTimer;

	UPROPERTY(EditAnywhere)
		float Spawn_X_Vel;

	UPROPERTY(EditAnywhere)
		float Spawn_Y_Vel;

	bool bHit;
	bool bStartDestroyTimer;

	void SpawnChildren(int32 Asteroid_Children);
	void SetHazardVelocity(FVector NewVelocity);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
