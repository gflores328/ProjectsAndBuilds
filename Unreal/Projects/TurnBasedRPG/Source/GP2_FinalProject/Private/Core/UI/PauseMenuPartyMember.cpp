// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/UI/PauseMenuPartyMember.h"

#include "PauseMenu.h"
#include "Components/Button.h"

void UPauseMenuPartyMember::NativeConstruct()
{
	Super::NativeConstruct();

	PartyMemberButton->OnClicked.AddDynamic(this, &UPauseMenuPartyMember::ButtonClicked);
}

void UPauseMenuPartyMember::NativeDestruct()
{
	Super::NativeDestruct();

	PartyMemberButton->OnClicked.RemoveAll(this);
}

void UPauseMenuPartyMember::ButtonClicked()
{
	if (UPauseMenu* PauseMenu = GetTypedOuter<UPauseMenu>())
	{
		PauseMenu->HealPlayer(PartyMemberIndex);
	}
}
