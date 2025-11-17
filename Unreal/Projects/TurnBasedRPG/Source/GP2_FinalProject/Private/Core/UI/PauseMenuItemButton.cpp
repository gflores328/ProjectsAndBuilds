// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/UI/PauseMenuItemButton.h"

#include "PauseMenu.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Core/Systems/FinalProject_GameInstance.h"
#include "Core/Systems/Item.h"
#include "GP2_FinalProject/GP2_FinalProjectCharacter.h"
#include "Kismet/GameplayStatics.h"

void UPauseMenuItemButton::NativeConstruct()
{
	Super::NativeConstruct();

	ItemButton->OnClicked.AddDynamic(this, &UPauseMenuItemButton::ButtonClicked);
}

void UPauseMenuItemButton::NativeDestruct()
{
	Super::NativeDestruct();

	ItemButton->OnClicked.RemoveAll(this);
}

void UPauseMenuItemButton::ButtonClicked()
{
	if (UPauseMenu* PauseMenu = GetTypedOuter<UPauseMenu>())
	{
		PauseMenu->ItemToUse = ItemData.Item;
		PauseMenu->ItemsMenu->SetVisibility(ESlateVisibility::Collapsed);
		PauseMenu->PartyMenu->SetVisibility(ESlateVisibility::Visible);

		if (auto* player = Cast<AGP2_FinalProjectCharacter> (UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
		{
			player->PopulatePartyMenu();
			UE_LOG(LogTemp, Warning, TEXT("Populate PartyMenu"));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Character null"));
		}
	}
}

void UPauseMenuItemButton::SetItemData(FItemData Item)
{
	ItemData = Item;

	FString Format = FString::Printf(TEXT("%s : x%d"), *Item.Item->ItemName.ToString(), Item.ItemQuantity);
	
	ItemText->SetText(FText::FromString(Format));
}
