// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/UI/TargetButton.h"

#include "Components/Button.h"
#include "Core/Characters/EnemyBattleCharacter.h"
#include "Core/Systems/CombatPawn.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"

void UTargetButton::NativeConstruct()
{
	Super::NativeConstruct();

	TargetButton->OnClicked.AddDynamic(this, &UTargetButton::OnClick);
}

void UTargetButton::OnClick()
{
	if ( auto* PlayerPawn = Cast<ACombatPawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0)))
	{
		PlayerPawn->Attack(Enemy);
	}
}
