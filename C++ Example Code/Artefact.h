// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Player/Interactable.h"
#include "../Items/InventoryComponent.h"
#include "Artefact.generated.h"

UCLASS()
class STEAMPROJECT_API AArtefact : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AArtefact();

public:	
	virtual void Interact(AActor* interactedActor) override;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
		FItemData Relic;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sound)
		class USoundBase* CollectItemSound;
};
