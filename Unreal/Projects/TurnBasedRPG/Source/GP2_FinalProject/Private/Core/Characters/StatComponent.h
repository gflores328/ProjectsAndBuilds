// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StatComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeathSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHealthChangedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMPChangedSignature);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStatComponent();

	UPROPERTY(BlueprintAssignable, Category = "Default") FOnHealthChangedSignature OnHealthChanged;
	UPROPERTY(BlueprintAssignable, Category = "Default") FOnDeathSignature OnDeath;
	UPROPERTY(BlueprintAssignable, Category = "Default") FOnMPChangedSignature OnMPChanged;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UFUNCTION() FORCEINLINE int GetCurrentHealth() const {return CurrentHealth;}
	UFUNCTION(BlueprintCallable) FORCEINLINE int GetMaxHealth() const {return MaxHealth;}
	UFUNCTION(BlueprintCallable) FORCEINLINE float GetHealthPercentage() const {return static_cast<float>(CurrentHealth) / MaxHealth;}
	UFUNCTION() FORCEINLINE int GetCurrentMP() const {return CurrentMP;}
	UFUNCTION() FORCEINLINE int GetMaxMP() const {return MaxMP;}
	UFUNCTION() FORCEINLINE float GetMPPercentage() const {return static_cast<float>(CurrentMP) / MaxMP;}
	UFUNCTION() FORCEINLINE int GetAgility() const {return AgilityValue;}
	UFUNCTION() FORCEINLINE int GetAttack() const {return AttackValue;}
	UFUNCTION() FORCEINLINE int GetDefense() const {return DefenseValue;}

	UFUNCTION() FORCEINLINE void SetCurrentHealth(int health) {CurrentHealth = health;}
	UFUNCTION() FORCEINLINE void SetCurrentMP(int MP) {CurrentMP = MP;}

	UFUNCTION() void UpdateHealth(int changeValue);
	UFUNCTION() void UpdateMP(int changeValue);
	
private:
	UPROPERTY(EditAnywhere, Category = "Stat") int CurrentHealth;
	UPROPERTY(EditAnywhere, Category = "Stat") int MaxHealth;
	UPROPERTY(EditAnywhere, Category = "Stat") int CurrentMP;
	UPROPERTY(EditAnywhere, Category = "Stat") int MaxMP;

	UPROPERTY(EditAnywhere, Category = "Stat") int AttackValue;
	UPROPERTY(EditAnywhere, Category = "Stat") int DefenseValue;
	UPROPERTY(EditAnywhere, Category = "Stat") int AgilityValue;
		
};
