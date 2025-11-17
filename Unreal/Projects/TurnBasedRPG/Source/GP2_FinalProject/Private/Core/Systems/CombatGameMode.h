// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CombatGameMode.generated.h"

/**
 * 
 */
UCLASS()
class ACombatGameMode : public AGameModeBase
{
	GENERATED_BODY()

	void AGameModeBase();

	
protected:
	virtual void BeginPlay() override;

	public:
};
