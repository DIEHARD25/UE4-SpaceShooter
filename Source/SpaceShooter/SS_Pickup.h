// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"


#include "SS_Pickup.generated.h"

UCLASS()
class SPACESHOOTER_API ASS_Pickup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASS_Pickup();


	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* PickUpMesh;

	UPROPERTY(EditAnywhere)
		UCapsuleComponent* CollisionComponent;

	UFUNCTION()
		void OnBeginOverlap(AActor* PickupActor, AActor* OtherActor);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
