// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Core/Systems/FinalProject_GameInstance.h"
#include "PauseMenuItemButton.generated.h"

struct FItemData;
class UTextBlock;
class UItem;
class UButton;
/**
 * 
 */
UCLASS()
class UPauseMenuItemButton : public UUserWidget
{
	GENERATED_BODY()
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	UFUNCTION() void ButtonClicked();
	
public:
	UPROPERTY() FItemData ItemData;
	UPROPERTY(meta = (BindWidget)) UButton* ItemButton;
	UPROPERTY(meta = (BindWidget)) UTextBlock* ItemText;

	UFUNCTION() void SetItemData(FItemData Item);
};
