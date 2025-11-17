// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Systems/FinalProject_GameInstance.h"

#include "Core/ExplorationActors/OverworldEnemy.h"

void UFinalProject_GameInstance::SetExplorationLocation(FVector Location)
{
	ExplorationPlayerLocation = Location;
}

FVector UFinalProject_GameInstance::GetExplorationLocation()
{
	return ExplorationPlayerLocation;
}

void UFinalProject_GameInstance::SetBattleEnemiesToSpawn(TArray<TSubclassOf<AEnemyBattleCharacter>> BattleEnemies)
{
	BattleEnemiesToSpawn = BattleEnemies;
}

TArray<TSubclassOf<AEnemyBattleCharacter>> UFinalProject_GameInstance::GetBattleEnemiesToSpawn()
{
	return BattleEnemiesToSpawn;
}

void UFinalProject_GameInstance::AddToDontLoadList(AOverworldEnemy* Enemy)
{
	DontLoad.Add(Enemy->GetName());
	
}
