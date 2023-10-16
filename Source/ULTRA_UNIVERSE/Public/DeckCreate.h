// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class ULTRA_UNIVERSE_API DeckCreate
{
private:
	//ÉfÉbÉLÇÃñáêî

public:
	DeckCreate();
	~DeckCreate();

	void CreatePassiveDeck();

	void CreateActiveDeck();
private:
	TArray<int> passiveDeck;
	TArray<int> activeDeck;
};
