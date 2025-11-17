// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Skill.generated.h"

class UNiagaraSystem;
/**
 * 
 */

UENUM()
enum class ESkillType
{
	Attack,
	Heal,
};

UCLASS()

class USkill : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere) FText SkillName;
	UPROPERTY(EditAnywhere) FText Description;
	UPROPERTY(EditAnywhere) ESkillType SkillType;
	UPROPERTY(EditAnywhere) int MPCost;
	UPROPERTY(EditAnywhere) int BasePower;
	UPROPERTY(EditAnywhere) UNiagaraSystem* HitEffect;
	
};
