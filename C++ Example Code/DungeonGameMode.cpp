// Fill out your copyright notice in the Description page of Project Settings.


#include "DungeonGameMode.h"

#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

#include "../Cards/BaseCard.h"
#include "../Player/IngameHUD.h"
#include "../SteamProjectCharacter.h"
#include "../Items/InventoryComponent.h"
#include "../Artefact/Artefact.h"
#include"../Global/LootManager.h"
#include "../Items/DestinationObject.h"

ADungeonGameMode::ADungeonGameMode() 
{
	currentBPM = 4;
	maxBPM = 60;
}

void ADungeonGameMode::BeginPlay() 
{
	//Create the loot singleton?
	GetLootInstance();
}

void ADungeonGameMode::PlayerUIMode()
{
	//Stop the player from moving
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC)
	{
		//sets input to UI
		FInputModeUIOnly InputMode;
		PC->SetInputMode(InputMode);
		PC->bShowMouseCursor = true;

		//Disables input for the player controller
		ASteamProjectCharacter* Character = Cast<ASteamProjectCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
		if (Character)
		{
			Character->DisablePlayerControllerInput();
		}
	}
}

#pragma region GameLoop
bool ADungeonGameMode::IsGameRunning()
{
	return bGameRunning;
}

int ADungeonGameMode::GetCurrentBPM()
{
	return currentBPM;
}

void ADungeonGameMode::ResetBPM()
{
	currentBPM = 4;
}

void ADungeonGameMode::StartGame()
{
	//GetLootInstance();
	bGameRunning = true;
	//Sets up the world Modifiers
	ModifyWorld(ModifyCard);
	//sets up the artefact destination;
	ASteamProjectCharacter* Character = Cast<ASteamProjectCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));

	if (!Character->Inventory->HasTarget()) 
	{
		TArray<AActor*> FoundArtefacts;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ArtefactClass, FoundArtefacts);

		if (FoundArtefacts.Num() > 0)
		{
			//assigns the artefact
			AArtefact* Artefact = Cast<AArtefact>(FoundArtefacts[0]);
			target->SetUp(Character, Artefact);

			//adds it to the inv system
			Character->Inventory->AddNavigationTool(target);
		}
	}

	//starts the timer loop
	TimerLoop();
}

void ADungeonGameMode::IncreaseTimerCounter(int amount = 5)
{
	if (BlockBPMIncreaseChance <= FMath::RandRange(0.0f, 1.0f)) 
	{
		currentBPM += amount;
	}
	else 
	{
		UE_LOG(LogTemp, Log, TEXT("Blocked Timer Increase"));
	}

	if (currentBPM > maxBPM)
		TimerRunOutLose();
}

void ADungeonGameMode::PlayerAtStart()
{
	//clears the timer and resets
	GetWorldTimerManager().ClearTimer(waitTimer);
	bGameRunning = false;
	ResetModifiers(ModifyCard);
}

void ADungeonGameMode::IncreaseBlockBPM(float amount)
{
	BlockBPMIncreaseChance += amount;
}

void ADungeonGameMode::ResetBlockBPM()
{
	BlockBPMIncreaseChance = 0.0f;
}

void ADungeonGameMode::TimerLoop()
{
	//plays the heartbeat sound
	if (HeartBeatSound != nullptr)
		UGameplayStatics::PlaySound2D(this, HeartBeatSound);

	//calculates the current bpm and how long to wait
	float waitTime = 60.0f / currentBPM;
	
	GetWorldTimerManager().SetTimer(waitTimer, this, &ADungeonGameMode::TimerLoop, waitTime);
}

void ADungeonGameMode::TimerRunOutLose()
{
	//clear the timer and stop the game
	GetWorldTimerManager().ClearTimer(waitTimer);
	bGameRunning = false;

	//initiate the end game UI here
	AIngameHUD* hud = Cast<AIngameHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
	hud->GameOver(FText::FromString("Time Ran Out!"));
	//put the player in UI mode
	PlayerUIMode();

}
#pragma endregion

#pragma region Modifiers
void ADungeonGameMode::ResetModifiers(UBaseCard* PlayersCard)
{
	PlayersCard->ResetCardModifier(GetWorld());
}

void ADungeonGameMode::ModifyWorld(UBaseCard* PlayersCard)
{
	UE_LOG(LogTemp, Log, TEXT("Modify Card"));

	PlayersCard->CardModify(GetWorld());
}
#pragma endregion

void ADungeonGameMode::PlayerIsDead()
{
	//create the end game UI here
	AIngameHUD* hud = Cast<AIngameHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
	hud->GameOver(FText::FromString("You Died!"));
	//put player in UI mode
	PlayerUIMode();
}

ALootManager * ADungeonGameMode::GetLootInstance()
{
	//if theres not a loot instance already
	if (lootInstance == nullptr) 
	{
		//search to see if there is one in the world
		TSubclassOf<ALootManager> FindClass;
		TArray<AActor*> FoundLootManagers;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), FindClass, FoundLootManagers);

		UE_LOG(LogTemp, Log, TEXT("Number of actors found, %i!"), FoundLootManagers.Num());
		//if you find an instance, set it to the loot instance
		if(FoundLootManagers.Num() > 0)
			lootInstance = Cast<ALootManager>(FoundLootManagers[0]);
		else 
		{
			//if not create a new instance of the loot manager
			FActorSpawnParameters spawnParams;
			spawnParams.Owner = this;

			lootInstance = GetWorld()->SpawnActor<ALootManager>(LootClass, FVector(0, 0, 0), FRotator(0, 0, 0), spawnParams);

			UE_LOG(LogTemp, Log, TEXT("New Loot System Created"));
		}
	}

	return lootInstance;
}
