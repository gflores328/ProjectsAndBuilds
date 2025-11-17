// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DamagePopup.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class UDamagePopup : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget)) UTextBlock* DamageText;

public:
	UFUNCTION() void SetText(FText NewText);
	
};
