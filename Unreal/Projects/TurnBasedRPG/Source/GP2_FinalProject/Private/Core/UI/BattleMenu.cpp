// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/UI/BattleMenu.h"

#include "Camera/CameraComponent.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Core/Systems/CombatPawn.h"
#include "Kismet/GameplayStatics.h"
#include "Core/UI/CombatPartyUI.h"

void UBattleMenu::NativeConstruct()
{
	Super::NativeConstruct();

	
	AttackButton->OnClicked.AddDynamic(this, &UBattleMenu::AttackButtonClicked);
	SkillButton->OnClicked.AddDynamic(this, &UBattleMenu::SkillButtonClicked);
	ItemButton->OnClicked.AddDynamic(this, &UBattleMenu::ItemButtonClicked);
	BackButton->OnClicked.AddDynamic(this, &UBattleMenu::BackButtonClicked);
}

void UBattleMenu::NativeDestruct()
{
	Super::NativeDestruct();

	AttackButton->OnClicked.RemoveDynamic(this, &UBattleMenu::AttackButtonClicked);
	SkillButton->OnClicked.RemoveDynamic(this, &UBattleMenu::SkillButtonClicked);
	ItemButton->OnClicked.RemoveDynamic(this, &UBattleMenu::ItemButtonClicked);
	BackButton->OnClicked.RemoveDynamic(this, &UBattleMenu::BackButtonClicked);
}

void UBattleMenu::AttackButtonClicked()
{
	if (auto* Player = Cast<ACombatPawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0)))
	{
		Player->bIsTargetting = true;
		Player->ActionType = EActionType::Attack;
		
		Player->GetCamera()->SetWorldRotation(Player->GetCamera()->GetComponentRotation() + FRotator(-20, 10, 0));
		Player->SetActorLocation(FVector(-100, -10, 390));

		TextDescription->SetText(FText::FromString(TEXT("Select a target")));
		DecriptionBorder->SetVisibility(ESlateVisibility::Visible);
		ActionsMenu->SetVisibility(ESlateVisibility::Collapsed);
		BackButton->SetVisibility(ESlateVisibility::Visible);
	}
}

void UBattleMenu::SkillButtonClicked()
{
	if (auto* Player = Cast<ACombatPawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0)))
	{
		ActionsMenu->SetVisibility(ESlateVisibility::Collapsed);

		Player->PopulateSkills();
		
		SkillListScroll->SetVisibility(ESlateVisibility::Visible);
		BackButton->SetVisibility(ESlateVisibility::Visible);
	}
	
}

void UBattleMenu::ItemButtonClicked()
{
	if (auto* Player = Cast<ACombatPawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0)))
	{
		ActionsMenu->SetVisibility(ESlateVisibility::Collapsed);

		Player->PopulateItems();
		
		ItemListScroll->SetVisibility(ESlateVisibility::Visible);
		BackButton->SetVisibility(ESlateVisibility::Visible);
	}

	
}

void UBattleMenu::BackButtonClicked()
{
	if (auto* Player = Cast<ACombatPawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0)))
	{
		Player->bIsTargetting = false;
		Player->SetActorLocation(Player->CombatParticipants[Player->TurnIndex]->GetActorLocation() + FVector(-180, 180, 100));
		Player->GetCamera()->SetWorldRotation(Player->InitialCameraRotation);
		
		ActionsMenu->SetVisibility(ESlateVisibility::Visible);
		ItemListScroll->SetVisibility(ESlateVisibility::Collapsed);
		SkillListScroll->SetVisibility(ESlateVisibility::Collapsed);
		DecriptionBorder->SetVisibility(ESlateVisibility::Collapsed);
		BackButton->SetVisibility(ESlateVisibility::Collapsed);
	}
}

