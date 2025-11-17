// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Core/Systems/FinalProject_GameInstance.h"
#include "ItemButton.generated.h"

struct FItemData;
class UItem;
class UTextBlock;
class UButton;
/**
 * 
 */
UCLASS()
class UItemButton : public UUserWidget
{
	GENERATED_BODY()

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	
	UFUNCTION() void ItemButtonClicked();

	UFUNCTION() void Hovered();
	UFUNCTION() void Unhovered();
public:
	UFUNCTION() void SetItemData(FItemData Item);
	
	UPROPERTY(meta=(BindWidget)) UButton* ItemButton;
	UPROPERTY(meta = (BindWidget)) UTextBlock* ItemText;
	UPROPERTY() FItemData ItemData;
};
