// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Characters/StatComponent.h"

// Sets default values for this component's properties
UStatComponent::UStatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UStatComponent::BeginPlay()
{
	Super::BeginPlay();

	//CurrentHealth = MaxHealth;
	
}


// Called every frame
void UStatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UStatComponent::UpdateHealth(int changeValue)
{
	CurrentHealth += changeValue;
	CurrentHealth = FMath::Clamp(CurrentHealth, 0.f, MaxHealth);

	OnHealthChanged.Broadcast();
	
	if (CurrentHealth <= 0)
	{
		OnDeath.Broadcast();
	}
}

void UStatComponent::UpdateMP(int changeValue)
{
	CurrentMP += changeValue;
	CurrentMP = FMath::Clamp(CurrentMP, 0.f, MaxMP);
	OnMPChanged.Broadcast();
}

