// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PauseMenuPartyMember.generated.h"

class UTextBlock;
class UButton;
/**
 * 
 */
UCLASS()
class UPauseMenuPartyMember : public UUserWidget
{
	GENERATED_BODY()

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UFUNCTION() void ButtonClicked();

public:
	UPROPERTY(meta = (BindWidget)) UButton* PartyMemberButton;
	UPROPERTY(meta = (BindWidget)) UTextBlock* PartyMemberName;
	UPROPERTY(meta = (BindWidget)) int PartyMemberIndex;
};
