// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/UI/PauseMenu.h"

#include "CombatPartyUI.h"
#include "PauseMenuItemButton.h"
#include "Components/Button.h"
#include "Components/VerticalBox.h"
#include "Core/Characters/PlayerBattleCharacter.h"
#include "Core/Characters/StatComponent.h"
#include "Core/Systems/FinalProject_GameInstance.h"
#include "Core/Systems/Item.h"
#include "GP2_FinalProject/GP2_FinalProjectCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Slate/SGameLayerManager.h"

void UPauseMenu::NativeConstruct()
{
	Super::NativeConstruct();

	ResumeButton->OnClicked.AddDynamic(this, &UPauseMenu::ResumeClick);
	QuitButton->OnClicked.AddDynamic(this, &UPauseMenu::QuitClick);
	HealButton->OnClicked.AddDynamic(this, &UPauseMenu::HealClick);
	BackButton->OnClicked.AddDynamic(this, &UPauseMenu::BackClick);
}

void UPauseMenu::NativeDestruct()
{
	Super::NativeDestruct();

	ResumeButton->OnClicked.RemoveAll(this);
	QuitButton->OnClicked.RemoveAll(this);
	HealButton->OnClicked.RemoveAll(this);
	BackButton->OnClicked.RemoveAll(this);
}

void UPauseMenu::ResumeClick()
{
	if (auto* playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		playerController->SetPause(false);
		
		FInputModeGameOnly Input;
		playerController->SetInputMode(Input);
		playerController->bShowMouseCursor = false;

		RemoveFromParent();
	}
}

void UPauseMenu::HealClick()
{
	PauseActions->SetVisibility(ESlateVisibility::Collapsed);
	ItemsMenu->SetVisibility(ESlateVisibility::Visible);
	BackButton->SetVisibility(ESlateVisibility::Visible);

	ItemsMenu->ClearChildren();

	if (auto* player = Cast<AGP2_FinalProjectCharacter> (UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
	{
		player->PopulatePauseItems();
	}
	
}

void UPauseMenu::QuitClick()
{
	if (auto* playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		UKismetSystemLibrary::QuitGame(GetWorld(), playerController, EQuitPreference::Quit, false);
	}
}

void UPauseMenu::BackClick()
{
	PauseActions->SetVisibility(ESlateVisibility::Visible);
	ItemsMenu->SetVisibility(ESlateVisibility::Collapsed);
	BackButton->SetVisibility(ESlateVisibility::Collapsed);
	PartyMenu->SetVisibility(ESlateVisibility::Collapsed);
}

void UPauseMenu::HealPlayer(int indexNum)
{
	if (auto* gameInstance = Cast<UFinalProject_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))) 
	{
		if (ItemToUse->ItemType == EItemType::Heal)
		{
			gameInstance->PartyMembers[indexNum].CurrentHealth += ItemToUse->ItemPower;
			gameInstance->PartyMembers[indexNum].CurrentHealth = FMath::Clamp(
				gameInstance->PartyMembers[indexNum].CurrentHealth,
				0.f,
				gameInstance->PartyMembersClasses[indexNum].GetDefaultObject()->StatComponent->GetMaxHealth());
		}
		else if (ItemToUse->ItemType == EItemType::MPHeal)
		{
			gameInstance->PartyMembers[indexNum].CurrentMP += ItemToUse->ItemPower;
		}

		int ItemIndex = gameInstance->Items.IndexOfByPredicate([&](auto& Inventory)
		{
			return Inventory.Item && Inventory.Item->ItemName.ToString() == ItemToUse->ItemName.ToString();
		});
		
		gameInstance->Items[ItemIndex].ItemQuantity -= 1;
	}
	
	PlayerHealthUI->SetSlotDataPauseMenu(indexNum);
	PartyMenu->SetVisibility(ESlateVisibility::Collapsed);
	ItemsMenu->SetVisibility(ESlateVisibility::Visible);
	ItemsMenu->ClearChildren();

	if (auto* player = Cast<AGP2_FinalProjectCharacter> (UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
	{
		player->PopulatePauseItems();
	}
}
