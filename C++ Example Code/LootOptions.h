// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "LootOptions.generated.h"

UENUM(BlueprintType)
enum ELootOptions
{
	Bad UMETA(DisplayName = "Bad Loot"),
	Common UMETA(DisplayName = "Common Loot"),
	Uncommon UMETA(DisplayName = "Uncommon Loot"),
	Rare UMETA(DisplayName = "Rare Loot"),
	Epic UMETA(DisplayName = "Epic Loot"),
};
