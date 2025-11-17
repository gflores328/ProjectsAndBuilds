// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "FinalProject_GameInstance.generated.h"

class AEnemyBattleCharacter;
class AOverworldEnemy;
class APlayerBattleCharacter;
class UItem;
/**
 * 
 */

USTRUCT()
struct FPartyMemberData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere) int CurrentHealth = 0;
	UPROPERTY(EditAnywhere) int CurrentMP = 0;
	
};

USTRUCT()
struct FItemData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere) UItem* Item;
	UPROPERTY(EditAnywhere) int ItemQuantity = 0;
};

UCLASS()
class UFinalProject_GameInstance : public UGameInstance
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere) TArray<TSubclassOf<AEnemyBattleCharacter>> BattleEnemiesToSpawn;

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere) TArray<TSubclassOf<APlayerBattleCharacter>> PartyMembersClasses;
	UPROPERTY(EditAnywhere) TArray<FPartyMemberData> PartyMembers;
	UPROPERTY(EditAnywhere) TArray<FString> DontLoad;
	UPROPERTY() FVector ExplorationPlayerLocation;
	UPROPERTY() FRotator ExplorationPlayerRotation;
	UPROPERTY(EditAnywhere) TArray<FItemData> Items;
	
	UFUNCTION() void SetExplorationLocation(FVector Location);
	UFUNCTION() FVector GetExplorationLocation();

	UFUNCTION() void SetBattleEnemiesToSpawn(TArray<TSubclassOf<AEnemyBattleCharacter>> BattleEnemies);
	UFUNCTION() TArray<TSubclassOf<AEnemyBattleCharacter>>  GetBattleEnemiesToSpawn();
	UFUNCTION() void AddToDontLoadList(AOverworldEnemy* Enemy);
};
