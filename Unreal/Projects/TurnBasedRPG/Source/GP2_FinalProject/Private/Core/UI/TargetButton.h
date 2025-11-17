// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TargetButton.generated.h"

class AEnemyBattleCharacter;
class UButton;
class UTextBlock;
/**
 * 
 */
UCLASS()
class UTargetButton : public UUserWidget
{
	GENERATED_BODY()
	
	virtual void NativeConstruct() override;
	UFUNCTION() void OnClick();
	
	UPROPERTY(meta = (BindWidget)) UButton* TargetButton;
	UPROPERTY(meta = (BindWidget)) UTextBlock* TargetText;
	UPROPERTY() AEnemyBattleCharacter* Enemy;
};
