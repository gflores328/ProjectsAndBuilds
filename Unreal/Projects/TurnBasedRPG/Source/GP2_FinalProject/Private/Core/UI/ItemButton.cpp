// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/UI/ItemButton.h"

#include "BattleMenu.h"
#include "Camera/CameraComponent.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "Core/Systems/CombatPawn.h"
#include "Core/Systems/FinalProject_GameInstance.h"
#include "Core/Systems/Item.h"
#include "Kismet/GameplayStatics.h"


void UItemButton::NativeConstruct()
{
	Super::NativeConstruct();

	ItemButton->OnClicked.AddDynamic(this, &UItemButton::ItemButtonClicked);
	ItemButton->OnHovered.AddDynamic(this, &UItemButton::Hovered);
	ItemButton->OnUnhovered.AddDynamic(this, &UItemButton::Unhovered);
}

void UItemButton::NativeDestruct()
{
	Super::NativeDestruct();

	ItemButton->OnClicked.RemoveDynamic(this, &UItemButton::ItemButtonClicked);
	ItemButton->OnHovered.RemoveDynamic(this, &UItemButton::Hovered);
	ItemButton->OnUnhovered.RemoveDynamic(this, &UItemButton::Unhovered);
}

void UItemButton::ItemButtonClicked()
{
	if (UBattleMenu* BattleMenu = GetTypedOuter<UBattleMenu>())
	{
		BattleMenu->ItemListScroll->SetVisibility(ESlateVisibility::Collapsed);
		
		ACombatPawn* Player = Cast<ACombatPawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
		if (ItemData.Item->ItemType == EItemType::Attack)
		{
			Player->CurrentItem = ItemData;
			Player->ActionType = EActionType::ItemHeal;
			Player->bIsTargetting = true;
		
			Player->GetCamera()->SetWorldRotation(Player->GetCamera()->GetComponentRotation() + FRotator(-20, 10, 0));
			Player->SetActorLocation(FVector(-100, -10, 390));

			ItemButton->OnUnhovered.RemoveDynamic(this, &UItemButton::Unhovered);
			BattleMenu->TextDescription->SetText(FText::FromString(TEXT("Select a target")));
			BattleMenu->DecriptionBorder->SetVisibility(ESlateVisibility::Visible);
		}
		else if (ItemData.Item->ItemType == EItemType::Heal || ItemData.Item->ItemType == EItemType::MPHeal)
		{
			Player->CurrentItem = ItemData;
			Player->ActionType = EActionType::ItemHeal;
			Player->bIsTargetting = true;

			Player->GetCamera()->SetWorldRotation(Player->GetCamera()->GetComponentRotation() + FRotator(0, 190, 0));
			Player->SetActorLocation(FVector(600, 0, 190));

			ItemButton->OnUnhovered.RemoveDynamic(this, &UItemButton::Unhovered);
			BattleMenu->TextDescription->SetText(FText::FromString(TEXT("Select a target")));
			BattleMenu->DecriptionBorder->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void UItemButton::Hovered()
{
	if (UBattleMenu* BattleMenu = GetTypedOuter<UBattleMenu>())
	{
		BattleMenu->TextDescription->SetText(ItemData.Item->ItemDescription);
		BattleMenu->DecriptionBorder->SetVisibility(ESlateVisibility::Visible);
		
	}
}

void UItemButton::Unhovered()
{
	if (UBattleMenu* BattleMenu = GetTypedOuter<UBattleMenu>())
	{
		BattleMenu->DecriptionBorder->SetVisibility(ESlateVisibility::Visible);
	}
}

void UItemButton::SetItemData(FItemData Item)
{
	ItemData = Item;

	FString FormattedText = FString::Printf(TEXT("%s : x%d"), *Item.Item->ItemName.ToString(), Item.ItemQuantity);

	ItemText->SetText(FText::FromString(FormattedText));
}
