// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/CapsuleComponent.h"

#include "Projectile.generated.h"

UCLASS()
class SPACESHOOTER_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent * BulletComponent;
	UPROPERTY(EditAnywhere)
		UCapsuleComponent * CapsuleCollision;
	UPROPERTY(EditAnywhere)
		float ProjectileSpeed;
	UFUNCTION()
		void OnBeginOverlap(AActor* ProjectileActor, AActor* OtherActor);

	void UpdateMovement(float DeltaTime);

	FVector CurrentLocation;
	bool bHit;
	bool bDestroy;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
