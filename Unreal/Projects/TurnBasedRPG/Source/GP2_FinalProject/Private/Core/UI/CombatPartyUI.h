// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CombatPartyUI.generated.h"

class APlayerBattleCharacter;
class UPlayerHealthMeter;
/**
 * 
 */
UCLASS()
class UCombatPartyUI : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget)) UPlayerHealthMeter* PartyMember1;
	UPROPERTY(meta = (BindWidget)) UPlayerHealthMeter* PartyMember2;
	UPROPERTY(meta = (BindWidget)) UPlayerHealthMeter* PartyMember3;

public:
	UFUNCTION() void SetSlotData(int slotNum, APlayerBattleCharacter* Player);
	UFUNCTION() void SetSlotDataPauseMenu(int slotNum);
	UFUNCTION() void UpdateHealth(int index);
	UFUNCTION() void UpdateMP(int index);
};
