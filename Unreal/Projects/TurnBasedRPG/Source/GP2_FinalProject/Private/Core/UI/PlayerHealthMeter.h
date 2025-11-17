// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHealthMeter.generated.h"

class APlayerBattleCharacter;
class UTextBlock;
class UProgressBar;
/**
 * 
 */
UCLASS()
class UPlayerHealthMeter : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION() void SetHealth(float newValue);
	UFUNCTION() void SetMP(float NewValue);
	UFUNCTION() void SetHealthPauseMenu(int slotNum);
	UFUNCTION() void SetMPPauseMenu(int slotNum);
	UFUNCTION() void SetData(APlayerBattleCharacter* PartyMember);
	UFUNCTION() void UpdateHealth();
	UFUNCTION() void UpdateMP();

	UPROPERTY(meta = (BindWidget)) UTextBlock* Name;
	UPROPERTY(meta = (BindWidget)) UProgressBar* HealthBar;
	UPROPERTY(meta = (BindWidget)) UProgressBar* MPBar;
	UPROPERTY(meta = (BindWidget)) UTextBlock* HPText;
	UPROPERTY(meta = (BindWidget)) UTextBlock* MPText;
	UPROPERTY(meta = (BindWidget)) UTextBlock* HealthValue;
	UPROPERTY(meta = (BindWidget)) UTextBlock* MPValue;

	UPROPERTY() APlayerBattleCharacter* PartyMemberAssociated;
};
