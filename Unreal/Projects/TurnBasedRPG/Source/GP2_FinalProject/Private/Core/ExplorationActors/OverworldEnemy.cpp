// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/ExplorationActors/OverworldEnemy.h"

#include "Core/Systems/FinalProject_GameInstance.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AOverworldEnemy::AOverworldEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
}

// Called when the game starts or when spawned
void AOverworldEnemy::BeginPlay()
{
	Super::BeginPlay();

	if (auto* gameInstance = Cast<UFinalProject_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld())))
	{
		if (gameInstance->DontLoad.Contains(GetName()))
		{
			Destroy();
		}
	}
}

// Called every frame
void AOverworldEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AOverworldEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

