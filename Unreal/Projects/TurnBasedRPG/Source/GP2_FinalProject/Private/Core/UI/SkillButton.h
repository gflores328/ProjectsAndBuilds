// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SkillButton.generated.h"

class USkill;
class UTextBlock;
class UButton;
/**
 * 
 */
UCLASS()
class USkillButton : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION() virtual void NativeConstruct() override;
	UFUNCTION() virtual void NativeDestruct() override;
	UFUNCTION() void SkillButtonClicked();
	UFUNCTION() void SetSkillData(USkill* skill);

	UFUNCTION() void Hovered();
	UFUNCTION() void Unhovered();

	UPROPERTY(meta = (BindWidget)) UButton* SkillButton;
	UPROPERTY(meta = (BindWidget)) UTextBlock* SkillText;
	UPROPERTY() USkill* SkillData;
	
};
