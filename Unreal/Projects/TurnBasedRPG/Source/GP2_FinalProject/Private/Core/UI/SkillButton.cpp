// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/UI/SkillButton.h"

#include "BattleMenu.h"
#include "Camera/CameraComponent.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "Core/Characters/PlayerBattleCharacter.h"
#include "Core/Characters/StatComponent.h"
#include "Core/Systems/CombatPawn.h"
#include "Core/Systems/Skill.h"
#include "Kismet/GameplayStatics.h"

void USkillButton::NativeConstruct()
{
	Super::NativeConstruct();

	SkillButton->OnClicked.AddDynamic(this, &USkillButton::SkillButtonClicked);
	SkillButton->OnHovered.AddDynamic(this, &USkillButton::Hovered);
	SkillButton->OnUnhovered.AddDynamic(this, &USkillButton::Unhovered);
}

void USkillButton::NativeDestruct()
{
	Super::NativeDestruct();
	SkillButton->OnClicked.RemoveDynamic(this, &USkillButton::SkillButtonClicked);
	SkillButton->OnHovered.RemoveDynamic(this, &USkillButton::Hovered);
	SkillButton->OnUnhovered.RemoveDynamic(this, &USkillButton::Unhovered);
}

void USkillButton::SkillButtonClicked()
{
	if (UBattleMenu* BattleMenu = GetTypedOuter<UBattleMenu>())
	{
		
		ACombatPawn* Player = Cast<ACombatPawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
		if (auto* CurrentPartyMember = Cast<APlayerBattleCharacter>(Player->CombatParticipants[Player->TurnIndex]))
		{
			if (SkillData->SkillType == ESkillType::Attack)
			{
				if (CurrentPartyMember->StatComponent->GetCurrentMP() > SkillData->MPCost)
				{
					BattleMenu->SkillListScroll->SetVisibility(ESlateVisibility::Collapsed);
					
					Player->SetCurrentSkill(SkillData);
					Player->ActionType = EActionType::Skill;
					Player->bIsTargetting = true;

					SkillButton->OnUnhovered.RemoveDynamic(this, &USkillButton::Unhovered);
					BattleMenu->TextDescription->SetText(FText::FromString(TEXT("Select a target")));
					BattleMenu->DecriptionBorder->SetVisibility(ESlateVisibility::Visible);

					Player->GetCamera()->SetWorldRotation(Player->GetCamera()->GetComponentRotation() + FRotator(-20, 10, 0));
					Player->SetActorLocation(FVector(-100, -10, 390));
				}
			}
			else if (SkillData->SkillType == ESkillType::Heal)
			{
				if (CurrentPartyMember->StatComponent->GetCurrentMP() > SkillData->MPCost)
				{
					BattleMenu->SkillListScroll->SetVisibility(ESlateVisibility::Collapsed);
					
					Player->SetCurrentSkill(SkillData);
					Player->ActionType = EActionType::Heal;
					Player->bIsTargetting = true;

					Player->GetCamera()->SetWorldRotation(Player->GetCamera()->GetComponentRotation() + FRotator(0, 190, 0));
					Player->SetActorLocation(FVector(600, 0, 190));

					SkillButton->OnUnhovered.RemoveDynamic(this, &USkillButton::Unhovered);
					BattleMenu->TextDescription->SetText(FText::FromString(TEXT("Select a target")));
					BattleMenu->DecriptionBorder->SetVisibility(ESlateVisibility::Visible);
				}
			}
		}
	}
}

void USkillButton::SetSkillData(USkill* skill)
{
	if (skill)
	{
		SkillData = skill;

		FString FormattedText = FString::Printf(TEXT("%s : %d"), *skill->SkillName.ToString(), skill->MPCost);
	
		SkillText->SetText(FText::FromString(FormattedText));
	}
}

void USkillButton::Hovered()
{
	if (UBattleMenu* BattleMenu = GetTypedOuter<UBattleMenu>())
	{
		BattleMenu->TextDescription->SetText(SkillData->Description);
		BattleMenu->DecriptionBorder->SetVisibility(ESlateVisibility::Visible);
		
	}
}

void USkillButton::Unhovered()
{
	if (UBattleMenu* BattleMenu = GetTypedOuter<UBattleMenu>())
	{
		BattleMenu->DecriptionBorder->SetVisibility(ESlateVisibility::Collapsed);
	}
}




