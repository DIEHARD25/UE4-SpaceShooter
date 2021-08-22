// Fill out your copyright notice in the Description page of Project Settings.


#include "SS_GameMode.h"

ASS_GameMode::ASS_GameMode()
{
	SpawnTimer = 0.0f;
	MusicTrack = CreateDefaultSubobject<UAudioComponent>(TEXT("MusicTrack"));
	MusicTrack->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	AsteroidSpawnLocation = FVector(1000.0f, 1000.0f, 1000.0f);
	AsteroidSpawnRotation = FRotator(0.0f, 0.0f,0.0f);

	ThisWorld = GetWorld();
}

void ASS_GameMode::BeginPlay()
{
	Super::BeginPlay();
	MusicTrack->Play();
	bPlayerDead = false;

	if (Score_Widget_Class)
	{
		Score_Widget = CreateWidget<UUserWidget>(GetWorld(), Score_Widget_Class);
		Score_Widget->AddToViewport();
	}

	if (Health_And_Armor_Class)
	{
		Health_And_Armor_Widget = CreateWidget<UUserWidget>(GetWorld(), Health_And_Armor_Class);
		Health_And_Armor_Widget->AddToViewport();
	}

	if (Restart_Class)
	{
		Restart_Widget = CreateWidget<UUserWidget>(GetWorld(), Restart_Class);
		Restart_Widget->AddToViewport();

		Restart_Widget->SetVisibility(ESlateVisibility::Hidden);
	}

	PC_Ref = GetWorld()->GetFirstPlayerController();


}

void ASS_GameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SpawnTimer = FMath::RandRange(0, 1000);

	if (Enemy && SpawnTimer > 990)
	{
		FVector EnemySpawnLoc(0.0f, 0.0f, 0.0f);
		FRotator EnemySpawnRot(0.0f, 0.0f, 0.0f);

		AActor* NewEnemy = GetWorld()->SpawnActor(Enemy, &EnemySpawnLoc, &EnemySpawnRot, AsteroidSpawnParams);

		if (NewEnemy)
			NewEnemy->SetActorLocation(FVector(1000.0f, FMath::RandRange(-500,500), 0.0f));	
	}

	if (HazardTemplate && SpawnTimer > 995)
		GetWorld()->SpawnActor(HazardTemplate, &AsteroidSpawnLocation, &AsteroidSpawnRotation, AsteroidSpawnParams);	

	if (bPlayerDead)
	{
		Restart_Widget->SetVisibility(ESlateVisibility::Visible);
		PC_Ref->bShowMouseCursor = true;
		bPlayerDead = false;
	}
}

void ASS_GameMode::RestartLevel(FName LevelName)
{
}

TSubclassOf<ASS_Hazard> ASS_GameMode::GetHazardTemplate()
{
	return TSubclassOf<ASS_Hazard>();
}