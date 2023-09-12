// Fill out your copyright notice in the Description page of Project Settings.


#include "JsonIO/JsonData.h"


AJsonDataObject::AJsonDataObject()
	: m_type(EJsonDataType::Null)
	, m_parent(nullptr)
	, m_data(TEXT("NULL"))
	, m_array{}
	, m_map{}
{
	// Set this actor to call Tick() every frame.You can turn this off to improve performance if you do not need it.

	PrimaryActorTick.bCanEverTick = true;

	// ...
}

