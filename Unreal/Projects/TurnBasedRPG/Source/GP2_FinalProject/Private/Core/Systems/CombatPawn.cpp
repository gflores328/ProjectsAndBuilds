// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Systems/CombatPawn.h"

#include "CharacterSpawner.h"
#include "DamagePopup.h"
#include "DamageTextActor.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "FinalProject_GameInstance.h"
#include "Item.h"
#include "Skill.h"
#include "Camera/CameraComponent.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/WidgetComponent.h"
#include "Core/Characters/EnemyBattleCharacter.h"
#include "Core/Characters/PlayerBattleCharacter.h"
#include "Core/Characters/StatComponent.h"
#include "Core/UI/BattleMenu.h"
#include "Core/UI/CombatPartyUI.h"
#include "Core/UI/GameOverUI.h"
#include "Core/UI/ItemButton.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Core/UI/SkillButton.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"

// Sets default values
ACombatPawn::ACombatPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	SceneRoot = CreateDefaultSubobject<USceneComponent>("SceneRoot");
	RootComponent = SceneRoot;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 0.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
	
}

// Called when the game starts or when spawned
void ACombatPawn::BeginPlay()
{
	Super::BeginPlay();
	

	if (auto* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		PlayerController->bShowMouseCursor = true;

		FInputModeGameAndUI Input;
		PlayerController->SetInputMode(Input);
	}

	if (const APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("BeginPlay"));
	
	if (BattleMenuClass)
	{
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		BattleMenu = CreateWidget<UBattleMenu>(PlayerController, BattleMenuClass);
	}

	if (GameOverClass)
	{
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		GameOverUI = CreateWidget<UGameOverUI>(PlayerController, GameOverClass);
	}

	InitialCameraRotation = FollowCamera->GetComponentRotation();
	
	InitializeBattle();
	BattleMenu->AddToViewport();
	NextTurn();
}

// Called to bind functionality to input
void ACombatPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		UE_LOG(LogTemp, Warning, TEXT("EnhancedInputComponent"));
		EnhancedInputComponent->BindAction(TargetSelectAction, ETriggerEvent::Triggered, this, &ACombatPawn::TargetSelect);
	}
}

void ACombatPawn::InitializeBattle()
{
	TArray<AActor*> ActorsFound;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), "CombatPlayer", ActorsFound);
	int characterIndex = 0;
	for (auto* FoundActor : ActorsFound)
	{
		ACharacterSpawner* FoundSpawner = Cast<ACharacterSpawner>(FoundActor);
		
		FVector SpawnLocation = FoundSpawner->GetActorLocation();
		FRotator SpawnRotation = FoundSpawner->GetActorRotation();
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		if (auto* gameInstance = Cast<UFinalProject_GameInstance>(GetWorld()->GetGameInstance()))
		{
			TSubclassOf<APlayerBattleCharacter> CharacterClass = gameInstance->PartyMembersClasses[characterIndex];
			ACharacter* SpawnedCharacter(GetWorld()->SpawnActor<APlayerBattleCharacter>(CharacterClass, SpawnLocation, SpawnRotation, SpawnParameters));

			CombatParticipants.Add(SpawnedCharacter);
		
			if (auto* found = Cast<APlayerBattleCharacter>(SpawnedCharacter))
			{

				if (gameInstance->PartyMembers[characterIndex].CurrentHealth < 1)
				{
					UE_LOG(LogTemp, Warning, TEXT("PlayerBattleCharacter has no Health"));
					
					gameInstance->PartyMembers[characterIndex].CurrentHealth = found->StatComponent->GetCurrentHealth();
					gameInstance->PartyMembers[characterIndex].CurrentMP = found->StatComponent->GetCurrentMP();
					
				}
				else
				{
					found->StatComponent->SetCurrentHealth(gameInstance->PartyMembers[characterIndex].CurrentHealth);
					found->StatComponent->SetCurrentMP(gameInstance->PartyMembers[characterIndex].CurrentMP);
				}
				
				PartyMembers.Add(found);	
			}
		}

		characterIndex++;
	}

	ActorsFound.Empty();
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), "CombatEnemy", ActorsFound);
	characterIndex = 0;
	for (auto* FoundActor : ActorsFound)
	{
		ACharacterSpawner* FoundSpawner = Cast<ACharacterSpawner>(FoundActor);
		
		FVector SpawnLocation = FoundSpawner->GetActorLocation();
		FRotator SpawnRotation = FoundSpawner->GetActorRotation();
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		if (auto* gameInstance = Cast<UFinalProject_GameInstance>(GetWorld()->GetGameInstance()))
		{
			if (gameInstance->GetBattleEnemiesToSpawn().Num() == characterIndex)
			{
				break;
			}

			ACharacter* SpawnedCharacter(GetWorld()->SpawnActor<AEnemyBattleCharacter>(gameInstance->GetBattleEnemiesToSpawn()[characterIndex], SpawnLocation, SpawnRotation, SpawnParameters));

			CombatParticipants.Add(SpawnedCharacter);
		}
		characterIndex++;
	}

	CombatParticipants.Sort( [](ACharacter& A, ACharacter& B)
	{
		return A.FindComponentByClass<UStatComponent>()->GetAgility() > B.FindComponentByClass<UStatComponent>()->GetAgility();
	});

	InitalizePartyUI();
}

void ACombatPawn::InitalizePartyUI()
{
	int i = 0;
	for (auto& Party : PartyMembers)
	{
		Party->UISlot = i;
		BattleMenu->CombatPartyUI->SetSlotData(i, Party);

		i += 1;
	}
}

void ACombatPawn::ShowEnemy()
{
	BattleMenu->CombatPartyUI->SetVisibility(ESlateVisibility::Collapsed);
	SetActorLocation(FVector(-360, 0, 100) + CombatParticipants[TurnIndex]->GetActorLocation());
	FollowCamera->SetWorldRotation(FRotator(0.f, 0, 0));

	GetWorld()->GetTimerManager().SetTimer(ActionTimer, this, &ACombatPawn::EnemyTurn, 1.0f, false);
}

void ACombatPawn::EnemyTurn()
{
	bool bValidCharacter = false;

	APlayerBattleCharacter* PlayerToAttack = PartyMembers[FMath::RandRange(0, PartyMembers.Num() - 1)];
	while (bValidCharacter == false)
	{

		if (CombatParticipants.Contains(PlayerToAttack))
		{
			bValidCharacter = true;
			break;
		}
		
		PlayerToAttack = PartyMembers[FMath::RandRange(0, PartyMembers.Num() - 1)];
	}
	
	if (auto* Enemy = Cast<AEnemyBattleCharacter>(CombatParticipants[TurnIndex]))
	{
		int Damage = (Enemy->StatComponent->GetAttack() - PlayerToAttack->StatComponent->GetDefense()) + FMath::RandRange(0, 10);

		if (Damage <= 0)
		{
			Damage = 1;
		}

		PlayerToAttack->StatComponent->UpdateHealth(Damage * -1);

		BattleMenu->CombatPartyUI->SetVisibility(ESlateVisibility::Visible);

		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), DefaultHitEffect, PlayerToAttack->GetActorLocation());
		
		ShowPlayerDamage(PlayerToAttack, Damage);
	
	}

	SetActorLocation(FVector(-320, 0, 260) + PlayerToAttack->GetActorLocation());
	FollowCamera->SetWorldRotation(FRotator(-30.f, 0, 0));
	
	GetWorld()->GetTimerManager().SetTimer(ActionTimer, this, &ACombatPawn::NextTurn, 2.0f, false);
	
	TurnIndex ++;
	//NextTurn();
}

void ACombatPawn::PlayerTurn()
{
	UE_LOG(LogTemp, Warning, TEXT("PlayerActionTaken"));

	TurnIndex ++;
	//BattleMenu->RemoveFromParent();
	NextTurn();
}

void ACombatPawn::NextTurn()
{
	FollowCamera->SetWorldRotation(InitialCameraRotation);

	bool bGameOver = false;

	for (auto participant : CombatParticipants)
	{
		if (auto* CheckedParticipant = Cast<AEnemyBattleCharacter>(participant))
		{
			bEndBattle = false;
			break;
		}
		else
		{
			bEndBattle = true;
		}
	}

	for (auto participant : PartyMembers)
	{
		if (CombatParticipants.Contains(participant))
		{
			bGameOver = false;
			break;
		}
		else
		{
			bGameOver = true;
		}
	}

	if (bGameOver)
	{
		GameOver();
		return;
	}

	if (bEndBattle)
	{
		EndBattle();
		return;
	}
	
	
	if (TurnIndex >= CombatParticipants.Num())
	{
		TurnIndex = 0;
	}

	if (auto* Player = Cast<APlayerBattleCharacter>(CombatParticipants[TurnIndex]))
	{
		RootComponent->SetWorldLocation((Player->GetActorLocation()) + FVector(-180, 180, 100));
		
		if (BattleMenu)
		{
			BattleMenu->ActionsMenu->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Menu not found"));
		}
	}
	else if (auto* Enemy = Cast<AEnemyBattleCharacter>(CombatParticipants[TurnIndex]))
	{
		ShowEnemy();
	}
}

void ACombatPawn::Attack(AEnemyBattleCharacter* EnemyToAttack)
{
	if (APlayerBattleCharacter* ControlledCharacter = Cast<APlayerBattleCharacter>(CombatParticipants[TurnIndex]))
	{
		int Damage = (ControlledCharacter->StatComponent->GetAttack() - EnemyToAttack->StatComponent->GetDefense()) + FMath::RandRange(0, 10);

		if (Damage <= 0)
		{
			Damage = 1;
		}
		
		EnemyToAttack->StatComponent->UpdateHealth(Damage * -1);
		bIsTargetting = false;
		BattleMenu->ActionsMenu->SetVisibility(ESlateVisibility::Collapsed);

		ShowDamage(EnemyToAttack, Damage);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), DefaultHitEffect, EnemyToAttack->GetActorLocation());

		GetWorld()->GetTimerManager().SetTimer(ActionTimer, this, &ACombatPawn::PlayerTurn, 2.0f, false);
		
	}
}

void ACombatPawn::TargetSelect()
{
	if (SelectionTarget)
	{
		
		switch (ActionType)
		{
		case EActionType::Attack:
			if (auto* target = Cast<AEnemyBattleCharacter>(SelectionTarget))
			{
				SetActorLocation(FVector(GetActorLocation().X, target->GetActorLocation().Y, GetActorLocation().Z));

				BattleMenu->DecriptionBorder->SetVisibility(ESlateVisibility::Collapsed);
				BattleMenu->BackButton->SetVisibility(ESlateVisibility::Collapsed);
				
				Attack(target);
			}
			break;

		case EActionType::Heal:
			if (auto* target = Cast<APlayerBattleCharacter>(SelectionTarget))
			{
				BattleMenu->DecriptionBorder->SetVisibility(ESlateVisibility::Collapsed);
				BattleMenu->BackButton->SetVisibility(ESlateVisibility::Collapsed);
				
				Heal(CurrentSkill,target);
				break;
			}

		case EActionType::ItemHeal:
			if (auto* target = Cast<APlayerBattleCharacter>(SelectionTarget))
			{
				BattleMenu->DecriptionBorder->SetVisibility(ESlateVisibility::Collapsed);
				BattleMenu->BackButton->SetVisibility(ESlateVisibility::Collapsed);
				
				ItemHeal(CurrentItem,target);
				break;
			}

		case EActionType::ItemAttack:
			if (auto* target = Cast<AEnemyBattleCharacter>(SelectionTarget))
			{
				BattleMenu->DecriptionBorder->SetVisibility(ESlateVisibility::Collapsed);
				BattleMenu->BackButton->SetVisibility(ESlateVisibility::Collapsed);
				
				SetActorLocation(FVector(GetActorLocation().X, target->GetActorLocation().Y, GetActorLocation().Z));
				ItemAttack(CurrentItem, target);
			}
			break;
			
		case EActionType::Skill:
			if (auto* target = Cast<AEnemyBattleCharacter>(SelectionTarget))
			{		
				BattleMenu->DecriptionBorder->SetVisibility(ESlateVisibility::Collapsed);
				BattleMenu->BackButton->SetVisibility(ESlateVisibility::Collapsed);
				
				SetActorLocation(FVector(GetActorLocation().X, target->GetActorLocation().Y, GetActorLocation().Z));
				AttackWithSkill(CurrentSkill, target);
			}
			break;

		default:
			break;
		}
	}
}

void ACombatPawn::UpdateHealth(int UISlot)
{
	BattleMenu->CombatPartyUI->UpdateHealth(UISlot);
}

void ACombatPawn::UpdateMP(int UISlot)
{
	BattleMenu->CombatPartyUI->UpdateMP(UISlot);
}

void ACombatPawn::Heal(USkill* Skill, APlayerBattleCharacter* PlayerToHeal)
{
	if (APlayerBattleCharacter* ControlledCharacter = Cast<APlayerBattleCharacter>(CombatParticipants[TurnIndex]))
	{
		UE_LOG(LogTemp, Error, TEXT("Heal Amount: %d"), Skill->BasePower);
		
		PlayerToHeal->StatComponent->UpdateHealth(Skill->BasePower);
		bIsTargetting = false;
		ControlledCharacter->StatComponent->UpdateMP(Skill->MPCost * -1);
		
		
		BattleMenu->ActionsMenu->SetVisibility(ESlateVisibility::Collapsed);
		BattleMenu->SkillListScroll->SetVisibility(ESlateVisibility::Collapsed);

		if (Skill->HitEffect)
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), Skill->HitEffect, PlayerToHeal->GetActorLocation());
		}

		GetWorld()->GetTimerManager().SetTimer(ActionTimer, this, &ACombatPawn::PlayerTurn, 2.0f, false);
		//PlayerTurn();
	}
}

void ACombatPawn::AttackWithSkill(USkill* Skill, AEnemyBattleCharacter* EnemyToAttack)
{
	if (APlayerBattleCharacter* ControlledCharacter = Cast<APlayerBattleCharacter>(CombatParticipants[TurnIndex]))
	{
		int Damage = (((ControlledCharacter->StatComponent->GetAttack() * Skill->BasePower) / 50) - EnemyToAttack->StatComponent->GetDefense()) + FMath::RandRange(0, 10);

		if (Damage <= 0)
		{
			Damage = 1;
		}
		
		EnemyToAttack->StatComponent->UpdateHealth(Damage * -1);
		bIsTargetting = false;
		

		BattleMenu->ActionsMenu->SetVisibility(ESlateVisibility::Collapsed);
		BattleMenu->SkillListScroll->SetVisibility(ESlateVisibility::Collapsed);

		ControlledCharacter->StatComponent->UpdateMP(Skill->MPCost * -1);

		ShowDamage(EnemyToAttack, Damage);

		if (Skill->HitEffect)
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), Skill->HitEffect, EnemyToAttack->GetActorLocation());
		}

		GetWorld()->GetTimerManager().SetTimer(ActionTimer, this, &ACombatPawn::PlayerTurn, 2.0f, false);
	}
}

void ACombatPawn::ItemHeal(FItemData Item, APlayerBattleCharacter* PlayerToHeal)
{
	bIsTargetting = false;
	
	if (Item.Item->ItemType == EItemType::Heal)
	{
		PlayerToHeal->StatComponent->UpdateHealth(Item.Item->ItemPower);
	}
	else if (Item.Item->ItemType == EItemType::MPHeal)
	{
		PlayerToHeal->StatComponent->UpdateMP(Item.Item->ItemPower);
	}

	BattleMenu->ActionsMenu->SetVisibility(ESlateVisibility::Collapsed);
	BattleMenu->ItemListScroll->SetVisibility(ESlateVisibility::Collapsed);

	if (auto* gameInstance = Cast<UFinalProject_GameInstance>(GetWorld()->GetGameInstance()))
	{
		int ItemIndex = gameInstance->Items.IndexOfByPredicate([&](auto& Inventory)
		{
			return Inventory.Item && Inventory.Item->ItemName.ToString() == Item.Item->ItemName.ToString();
		});
		
		gameInstance->Items[ItemIndex].ItemQuantity -= 1;
	}

	GetWorld()->GetTimerManager().SetTimer(ActionTimer, this, &ACombatPawn::PlayerTurn, 2.0f, false);
}

void ACombatPawn::ItemAttack(FItemData Item, AEnemyBattleCharacter* EnemyToAttack)
{
	bIsTargetting = false;
	
	EnemyToAttack->StatComponent->UpdateHealth(Item.Item->ItemPower * -1);
	
	BattleMenu->ActionsMenu->SetVisibility(ESlateVisibility::Collapsed);
	BattleMenu->ItemListScroll->SetVisibility(ESlateVisibility::Collapsed);

	if (auto* gameInstance = Cast<UFinalProject_GameInstance>(GetWorld()->GetGameInstance()))
	{
		int ItemIndex = gameInstance->Items.IndexOfByPredicate([&](auto& Inventory)
		{
			return Inventory.Item && Inventory.Item->ItemName.ToString() == Item.Item->ItemName.ToString();
		});
		
		gameInstance->Items[ItemIndex].ItemQuantity -= 1;
	}

	ShowDamage(EnemyToAttack, Item.Item->ItemPower);

	GetWorld()->GetTimerManager().SetTimer(ActionTimer, this, &ACombatPawn::PlayerTurn, 2.0f, false);
}

void ACombatPawn::SetCurrentSkill(USkill* Skill)
{
	CurrentSkill = Skill;
}

void ACombatPawn::PopulateSkills()
{
	BattleMenu->SkillListScroll->ClearChildren();

	if (APlayerBattleCharacter* ControlledCharacter = Cast<APlayerBattleCharacter>(CombatParticipants[TurnIndex]))
	{
		for (auto& skill : ControlledCharacter->Skills)
		{
			if (USkillButton* newButton = CreateWidget<USkillButton>(BattleMenu, SkillButtonClass))
			{
				newButton->SetSkillData(skill);
				BattleMenu->SkillListScroll->AddChild(newButton);
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Failed to create SkillButton widget"));
			}
		}
	}
}

void ACombatPawn::PopulateItems()
{
	BattleMenu->ItemListScroll->ClearChildren();

	if (auto* gameInstance = Cast<UFinalProject_GameInstance>(GetGameInstance()))
	{
		for (auto& item : gameInstance->Items)
		{
			if (item.ItemQuantity > 0)
			{
				if (UItemButton* newButton = CreateWidget<UItemButton>(BattleMenu, ItemButtonClass))
				{
					newButton->SetItemData(item);
					BattleMenu->ItemListScroll->AddChild(newButton);
				}
			}
		}
	}
}

UCameraComponent* ACombatPawn::GetCamera()
{
	return FollowCamera;
}

void ACombatPawn::ShowDamage(AEnemyBattleCharacter* Enemy, int Damage)
{
	FVector Location = Enemy->GetActorLocation();
	FRotator Rotation = Enemy->GetActorRotation();
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	ADamageTextActor* Popup = GetWorld()->SpawnActor<ADamageTextActor>(DamagePopup, Location + FVector(-30, 0, 30), Rotation, SpawnParameters);
	Popup->SetLifeSpan(1);
	
	if (auto* PopupWidget = Cast<UDamagePopup>(Popup->DamageWidget->GetUserWidgetObject()))
	{
		PopupWidget->SetText(FText::FromString(FString::FromInt(Damage)));
	}
}

void ACombatPawn::ShowPlayerDamage(APlayerBattleCharacter* Player, int Damage)
{
	FVector Location = Player->GetActorLocation() + FVector(-40, 0, 60);
	FRotator Rotation = Player->GetActorRotation() + FRotator(60, 180, 0);
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	ADamageTextActor* Popup = GetWorld()->SpawnActor<ADamageTextActor>(DamagePopup, Location, Rotation, SpawnParameters);
	Popup->SetLifeSpan(1);

	if (auto* PopupWidget = Cast<UDamagePopup>(Popup->DamageWidget->GetUserWidgetObject()))
	{
		PopupWidget->SetText(FText::FromString(FString::FromInt(Damage)));
	}
}

void ACombatPawn::RemovePlayerFromBattle(APlayerBattleCharacter* Player)
{
	int RemovedIndex = CombatParticipants.IndexOfByKey(Player);

	CombatParticipants.Remove(Player);

	Player->SetActorHiddenInGame(true);
	Player->SetActorEnableCollision(false);
	Player->SetActorTickEnabled(false);

	if (RemovedIndex <= TurnIndex && TurnIndex > 0)
	{
		TurnIndex--;
	}
}

void ACombatPawn::RemoveEnemyFromBattle(AEnemyBattleCharacter* Enemy)
{
	int RemovedIndex = CombatParticipants.IndexOfByKey(Enemy);

	CombatParticipants.Remove(Enemy);
	Enemy->Destroy();
	
	if (RemovedIndex <= TurnIndex && TurnIndex > 0)
	{
		TurnIndex--;
	}
}

void ACombatPawn::EndBattle()
{
	
	if (auto* gameInstance = Cast<UFinalProject_GameInstance>(GetWorld()->GetGameInstance()))
	{
		for (int32 i = 0; i < PartyMembers.Num(); ++i)
		{
			if (!PartyMembers[i])
			{
				UE_LOG(LogTemp, Warning, TEXT("PartyMember %d is null."), i);
				continue;
			}

			if (!PartyMembers[i]->StatComponent)
			{
				UE_LOG(LogTemp, Warning, TEXT("PartyMember %d has no StatComponent."), i);
				continue;
			}

			if (i >= gameInstance->PartyMembers.Num())
			{
				UE_LOG(LogTemp, Error, TEXT("GameInstance PartyMembers does not have index %d"), i);
				continue;
			}

			if (PartyMembers[i]->StatComponent->GetCurrentHealth() == 0)
			{
				gameInstance->PartyMembers[i].CurrentHealth = 1;
			}
			else
			{
				gameInstance->PartyMembers[i].CurrentHealth = PartyMembers[i]->StatComponent->GetCurrentHealth();
			}

			gameInstance->PartyMembers[i].CurrentMP = PartyMembers[i]->StatComponent->GetCurrentMP();
		}
	}

	if (auto* playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		playerController->bShowMouseCursor = false;
	}
	
	UGameplayStatics::OpenLevel(GetWorld(), "Exploration_Level");
}

void ACombatPawn::GameOver()
{
	GameOverUI->AddToViewport();
	GameOverUI->SetVisibility(ESlateVisibility::Visible);

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

	GetWorld()->GetTimerManager().SetTimer(ActionTimer, this, &ACombatPawn::LoadMainMenu, 5.0f, false);
}

void ACombatPawn::LoadMainMenu()
{
	UGameplayStatics::OpenLevel(GetWorld(), "MainMenu_Level");
}


// Called every frame
void ACombatPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	
	if (bIsTargetting)
	{
		FVector2d Mouse;
		FVector WorldLocation;
		FVector WorldDirection;
		
		if (auto* playerControler = UGameplayStatics::GetPlayerController(GetWorld(),0))
		{
			playerControler->GetMousePosition(Mouse.X, Mouse.Y);
			UGameplayStatics::DeprojectScreenToWorld(playerControler, Mouse, WorldLocation, WorldDirection);
		}
		
		FVector lineTraceStart = WorldLocation;
		FVector lineTraceEnd = lineTraceStart + WorldDirection * 5000;
		FCollisionQueryParams TraceParams;
		TraceParams.AddIgnoredActor(this);
		FHitResult hit(ForceInit);

		GetWorld()->LineTraceSingleByChannel(
			hit,
			lineTraceStart,
			lineTraceEnd,
			ECC_Pawn,
			TraceParams);

		if (auto* target = Cast<ACharacter>(hit.GetActor()))
		{
			SelectionTarget = target;
		}
	}
	else
	{
		SelectionTarget = nullptr;
	}
}


