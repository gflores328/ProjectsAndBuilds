// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Characters/EnemyBattleCharacter.h"

#include "StatComponent.h"
#include "Core/Systems/CombatPawn.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AEnemyBattleCharacter::AEnemyBattleCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StatComponent = CreateDefaultSubobject<UStatComponent>(TEXT("StatComponent"));
}

// Called when the game starts or when spawned
void AEnemyBattleCharacter::BeginPlay()
{
	Super::BeginPlay();

	StatComponent->OnDeath.AddDynamic(this, &AEnemyBattleCharacter::Die);
}

// Called every frame
void AEnemyBattleCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemyBattleCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemyBattleCharacter::Die()
{
	if (auto* player = Cast<ACombatPawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0)))
	{
		player->RemoveEnemyFromBattle(this);
	}
}

