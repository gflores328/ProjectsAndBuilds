// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BackButton.generated.h"

class UButton;
class UTextBlock;
/**
 * 
 */
UCLASS()
class UBackButton : public UUserWidget
{
	GENERATED_BODY()

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UPROPERTY(meta=(BindWidget)) UButton* BackButton;
	UPROPERTY(meta=(BindWidget)) UTextBlock* BackText;

	UFUNCTION() void BackButtonClicked();
};
