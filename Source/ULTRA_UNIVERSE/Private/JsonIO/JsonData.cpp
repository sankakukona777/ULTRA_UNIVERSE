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

void AJsonDataObject::Erase()
{
	AJsonDataObject* parent = dynamic_cast<AJsonDataObject*>(m_parent);
	if (parent != nullptr)
	{
		parent->HaveDataRemove(this);
	}
	m_parent = nullptr;

	this->Destroy();
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

//----------------------------------------------------------------------------------------------------------------------------------------

bool AJsonDataObject::HaveDataRemove(AJsonDataObject* child)
{
	switch (m_type)
	{
	case EJsonDataType::Array:
		if (m_array.Contains(child))
		{
			m_array.Remove(child);
			child->m_parent = nullptr;
			return true;
		}
		break;
	case EJsonDataType::Map:
		if (m_map.FindKey(child))
		{
			m_map.Remove(*m_map.FindKey(child));
			child->m_parent = nullptr;
			return true;
		}
		break;
	}
	return false;
}

bool AJsonDataObject::HaveDataErase(AJsonDataObject* child)
{
	switch (m_type)
	{
	case EJsonDataType::Array:
		if (m_array.Contains(child))
		{
			child->Erase();
			return true;
		}
		break;
	case EJsonDataType::Map:
		if (m_map.FindKey(child))
		{
			child->Erase();
			return true;
		}
		break;
	}
	return false;
}

//----------------------------------------------------------------------------------------------------------------------------------------

bool AJsonDataObject::ChangeMapTagDataObj(AJsonDataObject* child, const FString& afterTag)
{
	if (m_type != EJsonDataType::Map)
		return false;

	if (m_map.FindKey(child) && !m_map.Contains(afterTag))
	{
		m_map.Remove(*m_map.FindKey(child));
		m_map.Add(afterTag, child);
		return true;
	}

	return false;
}
bool AJsonDataObject::ChangeMapTagDataTag(const FString& beforeTag, const FString& afterTag)
{
	if (m_type != EJsonDataType::Map)
		return false;

	if (m_map.Contains(beforeTag) && !m_map.Contains(afterTag))
	{
		AJsonDataObject* child = *m_map.Find(beforeTag);
		m_map.Remove(beforeTag);
		m_map.Add(afterTag, child);
		return true;
	}

	return false;

}

bool AJsonDataObject::EraseMapTagData(const FString& eraseDataTag)
{
	if (m_type != EJsonDataType::Map)
		return false;

	if (m_map.Contains(eraseDataTag))
	{
		AJsonDataObject* erase = *m_map.Find(eraseDataTag);
		erase->Erase();
		return true;
	}

	return false;
}

//----------------------------------------------------------------------------------------------------------------------------------------
//****************************************************************************************************************************************
//----------------------------------------------------------------------------------------------------------------------------------------



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
