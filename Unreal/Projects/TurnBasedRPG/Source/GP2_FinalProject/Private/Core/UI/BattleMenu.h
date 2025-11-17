// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BattleMenu.generated.h"

class UBorder;
class UTextBlock;
class UVerticalBox;
class USkillButton;
class UScrollBox;
class UCombatPartyUI;
class UHorizontalBox;
class UButton;
/**
 * 
 */
UCLASS()
class UBattleMenu : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	
	UFUNCTION() void AttackButtonClicked();
	UFUNCTION() void SkillButtonClicked();
	UFUNCTION() void ItemButtonClicked();
	UFUNCTION() void BackButtonClicked();
public:

	UPROPERTY(meta = (BindWidget)) UVerticalBox* ActionsMenu;
	UPROPERTY(meta = (BindWidget)) UButton* AttackButton;
	UPROPERTY(meta = (BindWidget)) UButton* SkillButton;
	UPROPERTY(meta = (BindWidget)) UButton* ItemButton;
	UPROPERTY(meta = (BindWidget)) UButton* BackButton;

	UPROPERTY(meta = (BindWidget)) UBorder* DecriptionBorder;
	UPROPERTY(meta = (BindWidget)) UTextBlock* TextDescription;

	UPROPERTY(meta = (BindWidget)) UCombatPartyUI* CombatPartyUI;

	UPROPERTY(meta = (BindWidget)) UScrollBox* SkillListScroll;
	UPROPERTY(meta = (BindWidget)) UScrollBox* ItemListScroll;
};
