// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * ÉfÉbÉLÇçÏÇÈä÷êî
 */
class ULTRA_UNIVERSE_API DeckCreate
{
public:
	DeckCreate();
	~DeckCreate();

	UFUNCTION(BlueprintCallable, Category = "MyBPLibrary")
	 void CreatePassiveDeck(FString cardId);

	UFUNCTION(BlueprintCallable, Category="MyBPLibrary")
	
	 void CreateActiveDeck(FString cardId);
private:
	TArray<FString> m_passiveDeck;
	TArray<FString> m_activeDeck;
};
