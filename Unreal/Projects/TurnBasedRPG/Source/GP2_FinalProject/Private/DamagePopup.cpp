// Fill out your copyright notice in the Description page of Project Settings.


#include "DamagePopup.h"

#include "Components/TextBlock.h"


void UDamagePopup::SetText(FText NewText)
{
	DamageText->SetText(NewText);
}
