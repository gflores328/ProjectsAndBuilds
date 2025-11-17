// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/UI/CombatPartyUI.h"

#include "PlayerHealthMeter.h"
#include "Components/TextBlock.h"
#include "Core/Characters/PlayerBattleCharacter.h"
#include "Core/Systems/FinalProject_GameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Slate/SGameLayerManager.h"

void UCombatPartyUI::SetSlotData(int slotNum, APlayerBattleCharacter* Player)
{

	switch (slotNum)
	{
		case 0:
			PartyMember1->SetData(Player);
			break;
		case 1:
			PartyMember2->SetData(Player);
			break;
		case 2:
			PartyMember3->SetData(Player);
			break;
		default:
			break;
	}
}

void UCombatPartyUI::SetSlotDataPauseMenu(int slotNum)
{
	if (auto* gameInstance = Cast<UFinalProject_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld())))
	{
		switch (slotNum)
		{
			case 0:
				PartyMember1->SetHealthPauseMenu(slotNum);
				PartyMember1->SetMPPauseMenu(slotNum);
				PartyMember1->Name->SetText(FText::FromString(gameInstance->PartyMembersClasses[slotNum].GetDefaultObject()->Name));
				break;
			case 1:
				PartyMember2->SetHealthPauseMenu(slotNum);
				PartyMember2->SetMPPauseMenu(slotNum);
				PartyMember2->Name->SetText(FText::FromString(gameInstance->PartyMembersClasses[slotNum].GetDefaultObject()->Name));
				break;
			case 2:
				PartyMember3->SetHealthPauseMenu(slotNum);
				PartyMember3->SetMPPauseMenu(slotNum);
				PartyMember3->Name->SetText(FText::FromString(gameInstance->PartyMembersClasses[slotNum].GetDefaultObject()->Name));
				break;
			default:
				break;
		}
	}
}

void UCombatPartyUI::UpdateHealth(int index)
{
	switch (index)
	{
	case 0:
		PartyMember1->UpdateHealth();
		break;
	case 1:
		PartyMember2->UpdateHealth();
		break;
	case 2:
		PartyMember3->UpdateHealth();
		break;
	default:
		break;
	}
	
}

void UCombatPartyUI::UpdateMP(int index)
{
	switch (index)
	{
	case 0:
		PartyMember1->UpdateMP();
		break;
	case 1:
		PartyMember2->UpdateMP();
		break;
	case 2:
		PartyMember3->UpdateMP();
		break;
	default:
		break;
	}
}


