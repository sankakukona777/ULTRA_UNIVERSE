// Fill out your copyright notice in the Description page of Project Settings.


#include "DeckCreate.h"

DeckCreate::DeckCreate()
{
}

DeckCreate::~DeckCreate()
{
}

void DeckCreate::CreatePassiveDeck(FString cardId)
{
	m_passiveDeck.Add(cardId);
}

void DeckCreate::CreateActiveDeck(FString cardId)
{
	m_activeDeck.Add(cardId);
}
