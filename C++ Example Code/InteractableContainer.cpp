// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableContainer.h"

#include "SteamProject/SteamProjectCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

#include "../Items/Item.h"
#include "../Items/InventoryComponent.h"
#include "../GameModes/DungeonGameMode.h"
#include "../Global/LootManager.h"

// Sets default values
AInteractableContainer::AInteractableContainer()
{

}

void AInteractableContainer::Interact(AActor * interactedActor)
{
	//if this has already been looted return;
	if (bLootedContainer) return;

	UE_LOG(LogTemp, Log, TEXT("Interacted with a container!"));

	if (interactedActor != nullptr)
	{
		//check to see its a player thats interacted
		ASteamProjectCharacter* character = Cast<ASteamProjectCharacter>(interactedActor);
		if (character == nullptr) return;
		//Get the GameMode for the LootInstance
		ADungeonGameMode* DGM = Cast<ADungeonGameMode>(GetWorld()->GetAuthGameMode());
		if (!DGM) return;

		bLootedContainer = true;
		//an array of all the loot items recieved from the loot instance to give to the player
		TArray<FItemData> Loot = DGM->GetLootInstance()->GetLoot(LootClass);

		//NEED to check for inv space here, if no space spawn item as an entity in the world.

		//add declaired items to the inventory;
		for (int i = 0; i < Loot.Num(); i++) {
			UE_LOG(LogTemp, Log, TEXT("Adding item %i"), i);
			character->Inventory->AddItem(Loot[i]);
		}

		//play a sound
		if(ChestOpenSound)
			UGameplayStatics::PlaySoundAtLocation(this, ChestOpenSound, GetActorLocation());

		//Runs any Blueprint scripts
		Interacted();
	}
}

