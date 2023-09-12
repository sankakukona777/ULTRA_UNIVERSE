// Fill out your copyright notice in the Description page of Project Settings.


#include "JsonIO/JsonIO.h"

#include "Misc/Paths.h"
#include "Misc/FileHelper.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"

//AJsonDataObject* CreateJsonDataObject(const FString& jsonDataName, FJsonValue* jsonData, AActor* parent);
//void SetJsonDataArrayObject(AJsonDataObject* jsonDataObject, const TArray<TSharedPtr<FJsonValue>>& jsonDatas);
//void SetJsonDataObjectObject(AJsonDataObject* jsonDataObject,const TSharedPtr<FJsonObject>& jsonDatas);

AJsonDataObject* UJsonIO::CreateJsonDataObjectFromFJsonObject(const FString& jsonDataName, const TSharedPtr<FJsonObject>& jsonData,AActor* parent)
{
    FActorSpawnParameters spawnParam;
    //spawnParam.Name = FName(jsonDataName);
    spawnParam.Owner = parent;
    AJsonDataObject* result = parent->GetWorld()->SpawnActor<AJsonDataObject>(AJsonDataObject::StaticClass(), spawnParam);
    result->SetActorLabel(jsonDataName);

    result->m_parent = parent;

    //FAttachmentTransformRules fatr = FAttachmentTransformRules(EAttachmentRule::SnapToTarget,false);
    //result->AttachToActor(parent,fatr);
    
    SetJsonDataObjectObject(jsonDataName,result, jsonData);
    return result;
}
AJsonDataObject* UJsonIO::CreateJsonDataObject(const FString& jsonDataName, FJsonValue* jsonData, AActor* parent)
{
    FActorSpawnParameters spawnParam;
    //spawnParam.Name = FName(jsonDataName);
    spawnParam.Owner = parent;
    AJsonDataObject* result = parent->GetWorld()->SpawnActor<AJsonDataObject>(AJsonDataObject::StaticClass(), spawnParam);
    result->SetActorLabel(jsonDataName);

    result->m_parent = parent;

    //FAttachmentTransformRules fatr = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, false);
    //result->AttachToActor(parent, fatr);

    switch (jsonData->Type)
    {
    case EJson::String:
    case EJson::Number:
    case EJson::Boolean:
    {
        result->m_type = EJsonDataType::Data;
        result->m_data = jsonData->AsString();
    }
    break;

    case EJson::Array:
    {
        SetJsonDataArrayObject(jsonDataName,result, jsonData->AsArray());
    }
    break;

    case EJson::Object:
    {
        SetJsonDataObjectObject(jsonDataName,result, jsonData->AsObject());
    }
    break;

    default:
        break;
    }  

    return result;
}
void UJsonIO::SetJsonDataArrayObject(const FString& jsonDataName,AJsonDataObject* jsonDataObject,const TArray<TSharedPtr<FJsonValue>>& jsonDatas)
{
    jsonDataObject->m_type = EJsonDataType::Array;

    int count = 0;
    for (auto data : jsonDatas)
    {
        FString name = jsonDataName + TEXT("::Array");
        name += FString::FromInt(count);
        count++;
        jsonDataObject->m_array.Push(CreateJsonDataObject(name, data.Get(), jsonDataObject));
    }

}
void UJsonIO::SetJsonDataObjectObject(const FString& jsonDataName,AJsonDataObject* jsonDataObject,const TSharedPtr<FJsonObject>& jsonDatas)
{
    jsonDataObject->m_type = EJsonDataType::Map;

    for (auto datas : jsonDatas->Values)
    {
        jsonDataObject->m_map.Add(datas.Key, CreateJsonDataObject(jsonDataName + TEXT("::") + datas.Key, datas.Value.Get(), jsonDataObject));
    }

}

//----------------------------------------------------------------------------------------------------------------------------------------
//****************************************************************************************************************************************
//----------------------------------------------------------------------------------------------------------------------------------------


bool UJsonIO::SaveJsonData(const FString& savePath, AJsonDataObject* data)
{
    TSharedPtr<FJsonObject> JsonRootObject = MakeShareable(new FJsonObject);

    ////�ۑ���������
    //const FString SaveDate = FDateTime::UtcNow().ToString();
    //JsonRootObject->SetStringField("SaveDate", SaveDate);

    ////�L������
    //JsonRootObject->SetStringField("Character", CharName);

    ////���x����̃I�u�W�F�N�g�̖��O
    //TArray<TSharedPtr<FJsonValue>> ItemObjects;

    //for (AActor* obj : Items)
    //{
    //    ItemObjects.Add(MakeShareable(new FJsonValueString(obj->GetFullName())));
    //}
    //JsonRootObject->SetArrayField("Items", ItemObjects);

    ////���l�̔z��
    //TArray<TSharedPtr<FJsonValue>> ItemCountArray;
    //for (int i : Count)
    //{
    //    ItemCountArray.Add(MakeShareable(new FJsonValueNumber(i)));
    //}
    //JsonRootObject->SetArrayField("ItemCount", ItemCountArray);

    // FString��Json���������ނ��߂�Writer���쐬
    FString OutPutString;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutPutString);

    // Json��FString�ɏ�������
    FJsonSerializer::Serialize(JsonRootObject.ToSharedRef(), Writer);

    return FFileHelper::SaveStringToFile(OutPutString, *savePath);
}

bool UJsonIO::LoadJsonData(const FString& loadPath, const FString& loadJsonDataName,AActor* act, AJsonDataObject*& data)
{
    FString RawData;

    FString path = FPaths::ProjectDir() + loadPath;
    bool bLoadedFile = FFileHelper::LoadFileToString(RawData, *path);

    if (bLoadedFile)
    {
        // FJsonObject(Json�f�[�^�̓��ꕨ)���쐬
        TSharedPtr<FJsonObject> JsonRootObject = MakeShareable(new FJsonObject());

        // FString����Json��ǂݍ��ނ��߂�Reader���쐬
        TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(RawData);

        // Json�����񂩂�Json�I�u�W�F�N�g�ɓǂݍ���
        if (FJsonSerializer::Deserialize(JsonReader, JsonRootObject))
        {
            data = CreateJsonDataObjectFromFJsonObject(loadJsonDataName,JsonRootObject, act);
        }
        return true;
    }
    return false;
}