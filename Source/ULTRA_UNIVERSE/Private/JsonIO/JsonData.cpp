// Fill out your copyright notice in the Description page of Project Settings.


#include "JsonIO/JsonData.h"


AJsonDataObject::AJsonDataObject()
	: m_type(EJsonDataType::Null)
	, m_data(TEXT("NULL"))
	, m_array{}
	, m_map{}
	, m_parent(nullptr)
{
	// Set this actor to call Tick() every frame.You can turn this off to improve performance if you do not need it.

	PrimaryActorTick.bCanEverTick = true;

	// ...
}

AJsonDataObject* AJsonDataObject::Clone(AJsonDataObject* parent)
{
	AJsonDataObject* result = nullptr;


	AJsonDataObject* dataParent = parent;

	if(dataParent == nullptr)
		dataParent = dynamic_cast<AJsonDataObject*>(m_parent);

	if (dataParent == nullptr)
		return nullptr;

	result = CreateCloneObject(dataParent);


	if (dataParent != nullptr)
	{
		switch (dataParent->m_type)
		{
		case EJsonDataType::Array:
			dataParent->m_array.Push(result);
			break;
		case EJsonDataType::Map:
		{
			FString key = *dataParent->m_map.FindKey(this) + TEXT("*");
			dataParent->m_map.Add(key, result);
		}
		break;
		}
	}

	return result;
}

AJsonDataObject* AJsonDataObject::CreateCloneObject(AJsonDataObject* parent)
{
	//結果
	AJsonDataObject* result = nullptr;
	//スポーンする際の設定
	FActorSpawnParameters spawnParam;

	//親の設定
	if (parent != nullptr)
	{
		spawnParam.Owner = parent;
		result = parent->GetWorld()->SpawnActor<AJsonDataObject>(AJsonDataObject::StaticClass(), spawnParam);
	}
	else
	{
		spawnParam.Owner = m_parent;
		result = m_parent->GetWorld()->SpawnActor<AJsonDataObject>(AJsonDataObject::StaticClass(), spawnParam);
	}

	//アウトライナーの表示設定
	result->SetActorLabel(this->GetActorLabel() + TEXT("*"));

	//このデータの種類
	result->m_type = m_type;

	//データの設定
	switch (m_type)
	{
	case EJsonDataType::Data:
		result->m_data = m_data;
		break;
	case EJsonDataType::Array:
		for (auto data : m_array)
		{
			result->m_array.Push(data->CreateCloneObject(result));
		}
		break;
	case EJsonDataType::Map:
		for (auto data : m_map)
		{
			result->m_map.Add(data.Key + TEXT("*"), data.Value->CreateCloneObject(result));
		}
		break;
	}

	return result;
}
