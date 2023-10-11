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
		parent->RemoveHaveData(this);
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

bool AJsonDataObject::RemoveHaveData(AJsonDataObject* child)
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

bool AJsonDataObject::EraseHaveData(AJsonDataObject* child)
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

bool AJsonDataObject::MoveHaveData(AJsonDataObject* child, AJsonDataObject* destJsonDataObj, const FString& moveDataAfterTag)
{
	switch (m_type)
	{
	case EJsonDataType::Array:
	{
		switch (destJsonDataObj->m_type)
		{
		case EJsonDataType::Array:
			return MoveDataArrayToArray(child, destJsonDataObj);
		case EJsonDataType::Map:
			return MoveDataArrayToMap(child, destJsonDataObj,moveDataAfterTag);
		}
	}
	break;
	case EJsonDataType::Map:
	{
		switch (destJsonDataObj->m_type)
		{
		case EJsonDataType::Array:
			return MoveDataMapToArray(child, destJsonDataObj);
		case EJsonDataType::Map:
			return MoveDataMapToMap(child, destJsonDataObj, moveDataAfterTag);
		}
	}
	break;
	}

	return false;
}


//----------------------------------------------------------------------------------------------------------------------------------------

bool AJsonDataObject::MoveDataArrayToArray(AJsonDataObject* moveData, AJsonDataObject* destJsonDataObj)
{
	if (m_type != EJsonDataType::Array || destJsonDataObj->m_type != EJsonDataType::Array)
		return false;

	m_array.Remove(moveData);
	destJsonDataObj->m_array.Add(moveData);

	moveData->m_parent = destJsonDataObj;

	return true;
}

bool AJsonDataObject::MoveDataArrayToMap(AJsonDataObject* moveData, AJsonDataObject* destJsonDataObj, const FString& moveDataAfterTag)
{
	if (moveDataAfterTag.IsEmpty())
		return false;

	if (m_type != EJsonDataType::Array || destJsonDataObj->m_type != EJsonDataType::Map)
		return false;

	if (destJsonDataObj->m_map.Contains(moveDataAfterTag))
		return false;

	m_array.Remove(moveData);
	destJsonDataObj->m_map.Add(moveDataAfterTag, moveData);

	moveData->m_parent = destJsonDataObj;

	return true;
}

//----------------------------------------------------------------------------------------------------------------------------------------

bool AJsonDataObject::ChangeMapTagDataObj(AJsonDataObject* child, const FString& afterTag)
{
	if (afterTag.IsEmpty())
		return false;

	if (m_type != EJsonDataType::Map)
		return false;

	if (!m_map.FindKey(child)->IsEmpty() && !m_map.Contains(afterTag))
	{
		m_map.Remove(*m_map.FindKey(child));
		m_map.Add(afterTag, child);
		return true;
	}

	return false;
}
bool AJsonDataObject::ChangeMapTagDataTag(const FString& beforeTag, const FString& afterTag)
{
	if (beforeTag.IsEmpty() || afterTag.IsEmpty())
		return false;

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

bool AJsonDataObject::MoveTagDataMapToMap(const FString& moveDataTag, AJsonDataObject* destJsonDataObj, const FString& moveDataAfterTag)
{
	if (m_type != EJsonDataType::Map || destJsonDataObj->m_type != EJsonDataType::Map)
		return false;

	if (!m_map.Contains(moveDataTag) || destJsonDataObj->m_map.Contains(moveDataAfterTag))
		return false;

	AJsonDataObject* moveObj = *m_map.Find(moveDataTag);
	m_map.Remove(moveDataTag);
	destJsonDataObj->m_map.Add(moveDataAfterTag, moveObj);

	moveObj->m_parent = destJsonDataObj;

	return true;
}
bool AJsonDataObject::MoveTagDataMapToArray(const FString& moveDataTag, AJsonDataObject* destJsonDataObj)
{
	if (m_type != EJsonDataType::Map || destJsonDataObj->m_type != EJsonDataType::Array)
		return false;

	if (!m_map.Contains(moveDataTag))
		return false;

	AJsonDataObject* moveObj = *m_map.Find(moveDataTag);
	m_map.Remove(moveDataTag);
	destJsonDataObj->m_array.Add(moveObj);

	moveObj->m_parent = destJsonDataObj;

	return true;
}


bool AJsonDataObject::MoveDataMapToMap(AJsonDataObject* moveData, AJsonDataObject* destJsonDataObj, const FString& moveDataAfterTag)
{
	if (moveDataAfterTag.IsEmpty())
		return false;

	if (m_type != EJsonDataType::Map || destJsonDataObj->m_type != EJsonDataType::Map)
		return false;

	if (m_map.FindKey(moveData)->IsEmpty() || destJsonDataObj->m_map.Contains(moveDataAfterTag))
		return false;

	m_map.Remove(*m_map.FindKey(moveData));
	destJsonDataObj->m_map.Add(moveDataAfterTag, moveData);

	moveData->m_parent = destJsonDataObj;

	return true;
}
bool AJsonDataObject::MoveDataMapToArray(AJsonDataObject* moveData, AJsonDataObject* destJsonDataObj)
{
	if (m_type != EJsonDataType::Map || destJsonDataObj->m_type != EJsonDataType::Array)
		return false;

	if (m_map.FindKey(moveData)->IsEmpty())
		return false;

	m_map.Remove(*m_map.FindKey(moveData));
	destJsonDataObj->m_array.Add(moveData);

	moveData->m_parent = destJsonDataObj;

	return true;
}

//----------------------------------------------------------------------------------------------------------------------------------------
//****************************************************************************************************************************************
//----------------------------------------------------------------------------------------------------------------------------------------



AJsonDataObject* AJsonDataObject::CreateCloneObject(AJsonDataObject* parent)
{
	//����
	AJsonDataObject* result = nullptr;
	//�X�|�[������ۂ̐ݒ�
	FActorSpawnParameters spawnParam;

	//�e�̐ݒ�
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

	//�A�E�g���C�i�[�̕\���ݒ�
	result->SetActorLabel(this->GetActorLabel() + TEXT("*"));

	//���̃f�[�^�̎��
	result->m_type = m_type;

	//�f�[�^�̐ݒ�
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