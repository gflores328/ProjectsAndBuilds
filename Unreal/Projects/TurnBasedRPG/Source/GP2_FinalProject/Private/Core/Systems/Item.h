// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Item.generated.h"

/**
 * 
 */

UENUM()
enum class EItemType
{
	Attack,
	Heal,
	MPHeal
};

UCLASS()
class UItem : public UDataAsset
{
	GENERATED_BODY()

public: 
	UPROPERTY(EditAnywhere) FText ItemName;
	UPROPERTY(EditAnywhere) FText ItemDescription;
	UPROPERTY(EditAnywhere) int ItemQuantity;
	UPROPERTY(EditAnywhere) EItemType ItemType;
	UPROPERTY(EditAnywhere) int ItemPower;
	
};
