// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/UI/PlayerHealthMeter.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Core/Characters/PlayerBattleCharacter.h"
#include "Core/Characters/StatComponent.h"
#include "Core/Systems/FinalProject_GameInstance.h"
#include "Kismet/GameplayStatics.h"

void UPlayerHealthMeter::SetHealth(float newValue)
{
	HealthBar->SetPercent(newValue);

	FString formatted = FString::Printf(TEXT("%d / %d"), PartyMemberAssociated->StatComponent->GetCurrentHealth(), PartyMemberAssociated->StatComponent->GetMaxHealth());
	HealthValue->SetText(FText::FromString(formatted));
	
}

void UPlayerHealthMeter::SetMP(float newValue)
{
	MPBar->SetPercent(newValue);

	FString formatted = FString::Printf(TEXT("%d / %d"), PartyMemberAssociated->StatComponent->GetCurrentMP(), PartyMemberAssociated->StatComponent->GetMaxMP());
	MPValue->SetText(FText::FromString(formatted));
}

void UPlayerHealthMeter::SetHealthPauseMenu(int slotNum)
{
	if (auto* gameInstance = Cast<UFinalProject_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld())))
	{
		HealthBar->SetPercent(static_cast<float>(gameInstance->PartyMembers[slotNum].CurrentHealth) / static_cast<float>(gameInstance->PartyMembersClasses[slotNum].GetDefaultObject()->StatComponent->GetMaxHealth()));

		FString formatted = FString::Printf(TEXT("%d / %d"), gameInstance->PartyMembers[slotNum].CurrentHealth, gameInstance->PartyMembersClasses[slotNum].GetDefaultObject()->StatComponent->GetMaxHealth());
		HealthValue->SetText(FText::FromString(formatted));	
	}
}

void UPlayerHealthMeter::SetMPPauseMenu(int slotNum)
{
	if (auto* gameInstance = Cast<UFinalProject_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld())))
	{
		MPBar->SetPercent(static_cast<float>(gameInstance->PartyMembers[slotNum].CurrentMP) / static_cast<float>(gameInstance->PartyMembersClasses[slotNum].GetDefaultObject()->StatComponent->GetMaxMP()));

		FString formatted = FString::Printf(TEXT("%d / %d"), gameInstance->PartyMembers[slotNum].CurrentMP, gameInstance->PartyMembersClasses[slotNum].GetDefaultObject()->StatComponent->GetMaxMP());
		MPValue->SetText(FText::FromString(formatted));	
	}
}

void UPlayerHealthMeter::SetData(APlayerBattleCharacter* PartyMember)
{
	PartyMemberAssociated = PartyMember;
	
	Name->SetText(FText::FromString(PartyMember->Name));

	SetHealth(PartyMemberAssociated->StatComponent->GetHealthPercentage());
	SetMP(PartyMemberAssociated->StatComponent->GetMPPercentage());

	
}

void UPlayerHealthMeter::UpdateHealth()
{
	if (PartyMemberAssociated)
	{
		HealthBar->SetPercent(PartyMemberAssociated->StatComponent->GetHealthPercentage());

		FString formatted = FString::Printf(TEXT("%d / %d"), PartyMemberAssociated->StatComponent->GetCurrentHealth(), PartyMemberAssociated->StatComponent->GetMaxHealth());
		HealthValue->SetText(FText::FromString(formatted));
	}
}

void UPlayerHealthMeter::UpdateMP()
{
	if (PartyMemberAssociated)
	{
		MPBar->SetPercent(PartyMemberAssociated->StatComponent->GetMPPercentage());

		FString formatted = FString::Printf(TEXT("%d / %d"), PartyMemberAssociated->StatComponent->GetCurrentMP(), PartyMemberAssociated->StatComponent->GetMaxMP());
		MPValue->SetText(FText::FromString(formatted));
	}
}
