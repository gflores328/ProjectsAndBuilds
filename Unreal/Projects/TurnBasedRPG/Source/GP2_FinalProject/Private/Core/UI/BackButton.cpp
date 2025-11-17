// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/UI/BackButton.h"

#include "BattleMenu.h"
#include "Components/Button.h"

void UBackButton::NativeConstruct()
{
	Super::NativeConstruct();

	BackButton->OnClicked.AddDynamic(this, &UBackButton::BackButtonClicked);
}

void UBackButton::NativeDestruct()
{
	Super::NativeDestruct();

	BackButton->OnClicked.RemoveDynamic(this, &UBackButton::BackButtonClicked);
}

void UBackButton::BackButtonClicked()
{
	
}
