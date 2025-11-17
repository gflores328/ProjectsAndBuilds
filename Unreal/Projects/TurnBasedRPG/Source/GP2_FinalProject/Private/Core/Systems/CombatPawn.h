// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FinalProject_GameInstance.h"
#include "Core/Characters/EnemyBattleCharacter.h"
#include "GameFramework/Pawn.h"
#include "CombatPawn.generated.h"

struct FItemData;
class UItem;
class UItemButton;
class UGameOverUI;
class ADamageTextActor;
class USkillButton;
class USkill;
class APlayerBattleCharacter;
class UInputAction;
class UInputMappingContext;
class AEnemyBattleCharacter;
class UPlayerHealthMeter;
class UBattleMenu;
class UCameraComponent;
class USpringArmComponent;
class UNiagaraSystem;

UENUM()
enum class EActionType
{
	Attack,
	Heal,
	ItemAttack,
	ItemHeal,
	Skill
};

UCLASS()
class ACombatPawn : public APawn
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USceneComponent* SceneRoot;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* TargetSelectAction;

	FTimerHandle ActionTimer;

public:
	// Sets default values for this pawn's properties
	ACombatPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION() void InitializeBattle();
	UFUNCTION() void InitalizePartyUI();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/////////////////////
	UFUNCTION() void ShowEnemy();
	UFUNCTION() void EnemyTurn();
	UFUNCTION() void PlayerTurn();
	UFUNCTION() void NextTurn();
	UFUNCTION() void Attack(AEnemyBattleCharacter* EnemyToAttack);
	UFUNCTION() void TargetSelect();
	UFUNCTION() void UpdateHealth(int UISlot);
	UFUNCTION() void UpdateMP(int UISlot);
	UFUNCTION() void Heal(USkill* Skill, APlayerBattleCharacter* PlayerToHeal);
	UFUNCTION() void AttackWithSkill(USkill* Skill, AEnemyBattleCharacter* EnemyToAttack);
	UFUNCTION() void ItemHeal(FItemData Item, APlayerBattleCharacter* PlayerToHeal);
	UFUNCTION() void ItemAttack(FItemData Item, AEnemyBattleCharacter* EnemyToAttack);
	UFUNCTION() void SetCurrentSkill(USkill* Skill);
	UFUNCTION() void PopulateSkills();
	UFUNCTION() void PopulateItems();
	UFUNCTION() UCameraComponent* GetCamera();
	UFUNCTION() void ShowDamage(AEnemyBattleCharacter* Enemy, int Damage);
	UFUNCTION() void ShowPlayerDamage(APlayerBattleCharacter* Player, int Damage);
	UFUNCTION() void RemovePlayerFromBattle(APlayerBattleCharacter* Player);
	UFUNCTION() void RemoveEnemyFromBattle(AEnemyBattleCharacter* Enemy);
	UFUNCTION() void EndBattle();
	UFUNCTION() void GameOver();
	UFUNCTION() void LoadMainMenu();

	UPROPERTY(EditAnywhere) TArray<ACharacter*> CombatParticipants;
	UPROPERTY(EditAnywhere) TArray<APlayerBattleCharacter*> PartyMembers;
	
	
	UPROPERTY() bool bInCombat = true;
	UPROPERTY() bool bWaitingForAction = true;
	UPROPERTY() int TurnIndex = 0;

	UPROPERTY() UBattleMenu* BattleMenu;
	UPROPERTY(EditAnywhere) TSubclassOf<UBattleMenu> BattleMenuClass;

	UPROPERTY() UGameOverUI* GameOverUI;
	UPROPERTY(EditAnywhere) TSubclassOf<UGameOverUI>  GameOverClass;

	UPROPERTY(EditAnywhere) TSubclassOf<ADamageTextActor> DamagePopup;

	UPROPERTY() bool bIsTargetting = false;
	UPROPERTY() ACharacter* SelectionTarget;
	UPROPERTY() EActionType ActionType;

	UPROPERTY() USkill* CurrentSkill;
	UPROPERTY(EditAnywhere) TSubclassOf<USkillButton> SkillButtonClass;

	UPROPERTY() FItemData CurrentItem;
	UPROPERTY(EditAnywhere) TSubclassOf<UItemButton> ItemButtonClass;

	UPROPERTY() FRotator InitialCameraRotation;

	UPROPERTY() bool bEndBattle = false;

	UPROPERTY(EditAnywhere) UNiagaraSystem* DefaultHitEffect;
};
