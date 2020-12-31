// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Player/Interactable.h"
#include "../Global/LootOptions.h"
#include "InteractableContainer.generated.h"

UCLASS()
class STEAMPROJECT_API AInteractableContainer : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractableContainer();

public:
	UPROPERTY(EditDefaultsOnly, Category = Gameplay)
	TEnumAsByte<ELootOptions> LootClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Sound)
		class USoundBase* ChestOpenSound;

private:
	bool bLootedContainer = false;

public:	
	virtual void Interact(AActor* interactedActor) override;

	UFUNCTION(BlueprintImplementableEvent)
	void Interacted();

};
