// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Systems/WinTrigger.h"

#include "FinalProject_GameInstance.h"
#include "Components/BoxComponent.h"
#include "Core/Characters/PlayerBattleCharacter.h"
#include "Core/Characters/StatComponent.h"
#include "Core/UI/WinScreen.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GP2_FinalProject/GP2_FinalProjectCharacter.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AWinTrigger::AWinTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TriggerBox = CreateDefaultSubobject<UBoxComponent>("BoxComponent");
	TriggerBox->SetupAttachment(RootComponent);

	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AWinTrigger::OnOverlap);
}

// Called when the game starts or when spawned
void AWinTrigger::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWinTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWinTrigger::OnOverlap(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (auto* player = Cast<AGP2_FinalProjectCharacter>(OtherActor))
	{
		if (auto* gameInstance = Cast<UFinalProject_GameInstance>(GetWorld()->GetGameInstance()))
		{
			for (int i = 0; i < 3; i++)
			{
				gameInstance->PartyMembers[i].CurrentHealth = gameInstance->PartyMembersClasses[i].GetDefaultObject()->StatComponent->GetMaxHealth();
				gameInstance->PartyMembers[i].CurrentMP = gameInstance->PartyMembersClasses[i].GetDefaultObject()->StatComponent->GetMaxMP();
			}

			gameInstance->ExplorationPlayerLocation = FVector::ZeroVector;
			gameInstance->DontLoad.Empty();
		}
		
		player->WinScreen->AddToViewport();
		player->WinScreen->SetVisibility(ESlateVisibility::Visible);

		if (auto* Character = Cast<ACharacter>(OtherActor))
		{
			Character->GetCharacterMovement()->DisableMovement();
		}
		
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AWinTrigger::BackToMainMenu, 4.0f, false);
	}
}

void AWinTrigger::BackToMainMenu()
{
	UGameplayStatics::OpenLevel(GetWorld(), "MainMenu_Level");
}

