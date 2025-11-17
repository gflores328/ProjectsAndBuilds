// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WinScreen.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class UWinScreen : public UUserWidget
{
	GENERATED_BODY()
	
	UPROPERTY(meta = (BindWidget)) UTextBlock* WinText;
};
