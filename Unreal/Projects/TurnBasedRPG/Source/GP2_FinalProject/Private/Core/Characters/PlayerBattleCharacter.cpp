// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Characters/PlayerBattleCharacter.h"

#include "StatComponent.h"
#include "Core/Systems/CombatPawn.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APlayerBattleCharacter::APlayerBattleCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StatComponent = CreateDefaultSubobject<UStatComponent>(TEXT("StatComponent"));
	
}

// Called when the game starts or when spawned
void APlayerBattleCharacter::BeginPlay()
{
	Super::BeginPlay();

	StatComponent->OnHealthChanged.AddDynamic(this, &APlayerBattleCharacter::HealthChanged);
	StatComponent->OnMPChanged.AddDynamic(this, &APlayerBattleCharacter::MPChanged);
	StatComponent->OnDeath.AddDynamic(this, &APlayerBattleCharacter::Die);
	
}

// Called every frame
void APlayerBattleCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerBattleCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void APlayerBattleCharacter::HealthChanged()
{
	
	
	if (auto* pawn = Cast<ACombatPawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0)))
	{
		pawn->UpdateHealth(UISlot);
	}
}

void APlayerBattleCharacter::MPChanged()
{
	if (auto* pawn = Cast<ACombatPawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0)))
	{
		pawn->UpdateMP(UISlot);
	}
}

void APlayerBattleCharacter::Die()
{
	if (auto* player = Cast<ACombatPawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0)))
	{
		player->RemovePlayerFromBattle(this);
	}
}

