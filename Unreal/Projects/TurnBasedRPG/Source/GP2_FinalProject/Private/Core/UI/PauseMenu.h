// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PauseMenu.generated.h"

class UItem;
class UVerticalBox;
class UButton;
class UCombatPartyUI;
/**
 * 
 */
UCLASS()
class UPauseMenu : public UUserWidget
{
	GENERATED_BODY()
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UFUNCTION() void ResumeClick();
	UFUNCTION() void HealClick();
	UFUNCTION() void QuitClick();
	UFUNCTION() void BackClick();

public:
	UPROPERTY(meta = (BindWidget)) UVerticalBox* PauseActions;
	UPROPERTY(meta = (BindWidget)) UVerticalBox* ItemsMenu;
	UPROPERTY(meta = (BindWidget)) UVerticalBox* PartyMenu;
	UPROPERTY(meta = (BindWidget)) UCombatPartyUI* PlayerHealthUI;
	UPROPERTY(meta = (BindWidget)) UButton* ResumeButton;
	UPROPERTY(meta = (BindWidget)) UButton* HealButton;
	UPROPERTY(meta = (BindWidget)) UButton* QuitButton;
	UPROPERTY(meta = (BindWidget)) UButton* BackButton;
	
	UPROPERTY() UItem* ItemToUse;

	UFUNCTION() void HealPlayer(int indexNum);
};
