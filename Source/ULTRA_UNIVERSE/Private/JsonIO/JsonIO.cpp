// Fill out your copyright notice in the Description page of Project Settings.


#include "JsonIO/JsonIO.h"

#include "Misc/Paths.h"
#include "Misc/FileHelper.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"

#include "EditorActorFolders.h"


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
        FString name = jsonDataName + TEXT("/") + TEXT("Array");
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
        jsonDataObject->m_map.Add(datas.Key, CreateJsonDataObject(jsonDataName + TEXT("/") + datas.Key, datas.Value.Get(), jsonDataObject));
    }

}

//----------------------------------------------------------------------------------------------------------------------------------------
//****************************************************************************************************************************************
//----------------------------------------------------------------------------------------------------------------------------------------

bool UJsonIO::CreateSaveJsonData(AJsonDataObject* data, TSharedPtr<FJsonValue>& value)
{
    switch (data->m_type)
    {
    case EJsonDataType::Data:
    {
        value = MakeShareable(new FJsonValueString(data->m_data));
    }
    return true;

    case EJsonDataType::Array:
    {
        TArray<TSharedPtr<FJsonValue>> jArray;
        for (auto jsonData : data->m_array)
        {
            TSharedPtr<FJsonValue> v;

            if (CreateSaveJsonData(jsonData, v))
            {
                if (v.IsValid())
                    jArray.Push(v);
            }
            else
                return false;
        }

        value = MakeShareable(new FJsonValueArray(jArray));

    }
    return true;

    case EJsonDataType::Map:
    {
        TSharedPtr<FJsonObject> o = MakeShareable(new FJsonObject);

        for (auto jsonData:data->m_map)
        {
            TSharedPtr<FJsonValue> v;

            if (CreateSaveJsonData(jsonData.Value, v))
            {
                o->Values.Add(jsonData.Key, v);
            }
            else
                return false;
        }

        value = MakeShareable(new FJsonValueObject(o));
    }
    return true;

    }

    return false;
}

//----------------------------------------------------------------------------------------------------------------------------------------
//****************************************************************************************************************************************
//----------------------------------------------------------------------------------------------------------------------------------------

bool UJsonIO::SaveJsonData(const FString& savePath, AJsonDataObject* data)
{
    TSharedPtr<FJsonValue> v;

    if (!CreateSaveJsonData(data, v))
        return false;

    // FStringにJsonを書き込むためのWriterを作成
    FString OutPutString;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutPutString);

    TSharedPtr<FJsonObject> o = v->AsObject();

    // JsonをFStringに書き込み
    FJsonSerializer::Serialize(o.ToSharedRef(), Writer);

    FString path = FPaths::ProjectDir() + savePath;
    return FFileHelper::SaveStringToFile(OutPutString, *path);
}

bool UJsonIO::LoadJsonData(const FString& loadPath, const FString& loadJsonDataName,AActor* act, AJsonDataObject*& data)
{
    FString RawData;

    FString path = FPaths::ProjectDir() + loadPath;
    bool bLoadedFile = FFileHelper::LoadFileToString(RawData, *path);

    if (bLoadedFile)
    {
        // FJsonObject(Jsonデータの入れ物)を作成
        TSharedPtr<FJsonObject> JsonRootObject = MakeShareable(new FJsonObject());

        // FStringからJsonを読み込むためのReaderを作成
        TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(RawData);

        // Json文字列からJsonオブジェクトに読み込み
        if (FJsonSerializer::Deserialize(JsonReader, JsonRootObject))
        {
            data = CreateJsonDataObjectFromFJsonObject(loadJsonDataName,JsonRootObject, act);
        }
        return true;
    }
    return false;
}