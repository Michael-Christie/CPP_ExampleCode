// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DungeonGameMode.generated.h"

/**
 * 
 */
UCLASS()
class STEAMPROJECT_API ADungeonGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ADungeonGameMode();

	virtual void BeginPlay() override;
	
	void PlayerUIMode();

		//Game Running
protected:
	bool bGameRunning = false;
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Instanced)
		class UDestinationObject* target;
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class AArtefact> ArtefactClass;

public:
	bool IsGameRunning();

	//Timer Function
protected:
	int currentBPM;
	int maxBPM;

	float BlockBPMIncreaseChance = 0.5f;

	FTimerHandle waitTimer;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class USoundBase* HeartBeatSound;
public:
	int GetCurrentBPM();
	void ResetBPM();

	void StartGame();
	void IncreaseTimerCounter(int amount);
	void PlayerAtStart();

	void IncreaseBlockBPM(float amount);
	void ResetBlockBPM();

protected:
	UFUNCTION()
	void TimerLoop();

	void TimerRunOutLose();

	//Apply Card Modifiers
public:
	UPROPERTY(EditDefaultsOnly, Category = Gameplay, Instanced)
		class UBaseCard* ModifyCard;

	void ResetModifiers(class UBaseCard* PlayersCard);
	void ModifyWorld(class UBaseCard* PlayersCards);

	//Players Health Death
public:
	void PlayerIsDead();

	//Loot System
public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class ALootManager> LootClass;
private:
	class ALootManager* lootInstance;
public:
	class ALootManager* GetLootInstance();
	
};
