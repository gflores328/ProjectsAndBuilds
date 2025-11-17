// Copyright Epic Games, Inc. All Rights Reserved.

#include "GP2_FinalProjectCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Core/Characters/PlayerBattleCharacter.h"
#include "Core/ExplorationActors/OverworldEnemy.h"
#include "Kismet/GameplayStatics.h"
#include "Core/Systems/FinalProject_GameInstance.h"
#include "Core/Systems/Item.h"
#include "Core/UI/CombatPartyUI.h"
#include "Core/UI/WinScreen.h"
#include "Core/UI/PauseMenu.h"
#include "Core/UI/PauseMenuItemButton.h"
#include "Core/UI/PauseMenuPartyMember.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AGP2_FinalProjectCharacter

AGP2_FinalProjectCharacter::AGP2_FinalProjectCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
	
}

void AGP2_FinalProjectCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}

		FInputModeGameOnly Input;
		PlayerController->SetInputMode(Input);
		PlayerController->bShowMouseCursor = false;
	}

	GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &AGP2_FinalProjectCharacter::OnHit);

	if (auto* gameInstance = Cast<UFinalProject_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld())))
	{
		if (gameInstance->ExplorationPlayerLocation != FVector::ZeroVector)
		{
			SetActorLocation(gameInstance->ExplorationPlayerLocation);
			SetActorRotation(gameInstance->ExplorationPlayerRotation);
		}

		for (auto character : gameInstance->PartyMembers)
		{
			
		}
	}

	if (WinScreenClass)
	{
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		WinScreen = CreateWidget<UWinScreen>(PlayerController, WinScreenClass);
	}

	if (PauseMenuClass)
	{
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		PauseMenu = CreateWidget<UPauseMenu>(PlayerController, PauseMenuClass);
	}
}

void AGP2_FinalProjectCharacter::PopulatePauseItems()
{
	if (auto* gameInstance = Cast<UFinalProject_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld())))
	{
		for (auto& item : gameInstance->Items)
		{
			if (item.Item->ItemType == EItemType::Heal || item.Item->ItemType == EItemType::MPHeal)
			{
				if (item.ItemQuantity > 0)
				{
					if (UPauseMenuItemButton* NewButton = CreateWidget<UPauseMenuItemButton>(PauseMenu, PauseMenuItemClass))
					{
						NewButton->SetItemData(item);
						PauseMenu->ItemsMenu->AddChild(NewButton);
						
					}
				}
			}
		}
	}
}

void AGP2_FinalProjectCharacter::PopulatePartyMenu()
{
	PauseMenu->PartyMenu->ClearChildren();
	
	if (auto* gameInstance = Cast<UFinalProject_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld())))
	{
		for (int i = 0; i < 3; i++)
		{
			UE_LOG(LogTemp, Warning, TEXT("Party Button created"));
			if (UPauseMenuPartyMember* NewButton = CreateWidget<UPauseMenuPartyMember>(PauseMenu, PausePartyMemberClass))
			{
				NewButton->PartyMemberIndex = i;
				NewButton->PartyMemberName->SetText(FText::FromString(gameInstance->PartyMembersClasses[i].GetDefaultObject()->Name));

				PauseMenu->PartyMenu->AddChild(NewButton);

				
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void AGP2_FinalProjectCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		//EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		//EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AGP2_FinalProjectCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AGP2_FinalProjectCharacter::Look);

		EnhancedInputComponent->BindAction(PauseAction, ETriggerEvent::Triggered, this, &AGP2_FinalProjectCharacter::Pause);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AGP2_FinalProjectCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AGP2_FinalProjectCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AGP2_FinalProjectCharacter::Pause(const FInputActionValue& Value)
{
	if (auto* PlayerController = Cast<APlayerController>(Controller))
	{
		PauseMenu->AddToViewport();
		
		if (auto* gameInstance = Cast<UFinalProject_GameInstance>(PlayerController->GetGameInstance()))
		{
			for (int i = 0; i < 3; i++)
			{
				PauseMenu->PlayerHealthUI->SetSlotDataPauseMenu(i);	
			}
		}
		
		PlayerController->bShowMouseCursor = true;
		
		FInputModeUIOnly Input;
		PlayerController->SetInputMode(Input);
		
		PlayerController->Pause();
	}
}

void AGP2_FinalProjectCharacter::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (auto* Enemy = Cast<AOverworldEnemy>(OtherActor))
	{
		if (auto* gameInstance = Cast<UFinalProject_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld())))
		{
			gameInstance->SetBattleEnemiesToSpawn(Enemy->EnemiesToSpawn);
			gameInstance->AddToDontLoadList(Enemy);

			gameInstance->ExplorationPlayerLocation = GetActorLocation();
			gameInstance->ExplorationPlayerRotation = GetActorRotation();
		}
		
		UGameplayStatics::OpenLevel(GetWorld(), "/Game/MyContent/Levels/Combat_Level");
	}
}
