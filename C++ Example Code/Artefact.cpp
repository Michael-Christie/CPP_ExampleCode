// Fill out your copyright notice in the Description page of Project Settings.


#include "Artefact.h"

#include "Kismet/GameplayStatics.h"

#include "SteamProject/SteamProjectCharacter.h"
#include "../Items/Item.h"
#include "../Items/DestinationObject.h"

// Sets default values
AArtefact::AArtefact()
{

}

void AArtefact::Interact(AActor* interactedActor)
{
	UE_LOG(LogTemp, Log, TEXT("Picked up Artefact!"));

	if (interactedActor != nullptr)
	{
		//check to see if the actor that overlapped was the player character
		ASteamProjectCharacter* character = Cast<ASteamProjectCharacter>(interactedActor);

		if (character == nullptr) return;

		//if the player characters destination is this artefact
		if (character->Inventory->GetTargetArtefact() == this)
		{
			//remove the nav tool and add the relic item to the players inventory
			character->Inventory->RemoveNavigationTool();
			character->Inventory->AddItem(Relic);

			//play a sound
			if(CollectItemSound)
				UGameplayStatics::PlaySoundAtLocation(this, CollectItemSound, GetActorLocation());

			//DESTROYS THE OBJECT AFTERWARDS
			Destroy();
		}

	}
}

