// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Systems/DamageTextActor.h"

#include "DamagePopup.h"
#include "Components/TextBlock.h"
#include "Components/WidgetComponent.h"

// Sets default values
ADamageTextActor::ADamageTextActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DamageWidget = CreateDefaultSubobject<UWidgetComponent>("DamageWidget");
	DamageWidget->SetupAttachment(GetRootComponent());
	

	ConstructorHelpers::FClassFinder<UUserWidget> WidgetClass(TEXT("/Game/MyContent/Blueprints/UI/WBP_DamagePopup"));
	DamageWidget->SetWidgetClass(WidgetClass.Class);
}

// Called when the game starts or when spawned
void ADamageTextActor::BeginPlay()
{
	Super::BeginPlay();

	DamageWidget->CastShadow = false;
	DamageWidget->bCastDynamicShadow = false;
	DamageWidget->bCastStaticShadow = false;
}

// Called every frame
void ADamageTextActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

