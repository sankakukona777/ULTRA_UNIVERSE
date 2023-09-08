// Fill out your copyright notice in the Description page of Project Settings.


#include "JsonIO/JsonIO.h"

#include "Misc/Paths.h"
#include "Misc/FileHelper.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"

bool UJsonIO::SaveJsonData(const FString& savePath,FString CharName, TArray<AActor*> Items, TArray<int> Count)
{
    JsonObjectPtr JsonRootObject = MakeShareable(new FJsonObject);

    //�ۑ���������
    const FString SaveDate = FDateTime::UtcNow().ToString();
    JsonRootObject->SetStringField("SaveDate", SaveDate);

    //�L������
    JsonRootObject->SetStringField("Character", CharName);

    //���x����̃I�u�W�F�N�g�̖��O
    TArray<TSharedPtr<FJsonValue>> ItemObjects;

    for (AActor* obj : Items)
    {
        ItemObjects.Add(MakeShareable(new FJsonValueString(obj->GetFullName())));
    }
    JsonRootObject->SetArrayField("Items", ItemObjects);

    //���l�̔z��
    TArray<TSharedPtr<FJsonValue>> ItemCountArray;
    for (int i : Count)
    {
        ItemCountArray.Add(MakeShareable(new FJsonValueNumber(i)));
    }
    JsonRootObject->SetArrayField("ItemCount", ItemCountArray);

    // FString��Json���������ނ��߂�Writer���쐬
    FString OutPutString;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutPutString);

    // Json��FString�ɏ�������
    FJsonSerializer::Serialize(JsonRootObject.ToSharedRef(), Writer);

    return FFileHelper::SaveStringToFile(OutPutString, *savePath);
}

bool UJsonIO::LoadJsonData(const FString& loadPath, FString& CharName, TArray<FString>& Items, TArray<int>& Count)
{
    FString RawData;

    FString path = FPaths::ProjectDir() + loadPath;
    bool bLoadedFile = FFileHelper::LoadFileToString(RawData, *path);

    if (bLoadedFile)
    {
        // FJsonObject(Json�f�[�^�̓��ꕨ)���쐬
        JsonObjectPtr JsonRootObject = MakeShareable(new FJsonObject());

        // FString����Json��ǂݍ��ނ��߂�Reader���쐬
        TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(RawData);

        // Json�����񂩂�Json�I�u�W�F�N�g�ɓǂݍ���
        if (FJsonSerializer::Deserialize(JsonReader, JsonRootObject))
        {
            //�L�����l�[���̎擾
            CharName = JsonRootObject->GetStringField("Character");

            //���x����̃I�u�W�F�N�g�̎擾
            for (TSharedPtr<FJsonValue> V : JsonRootObject->GetArrayField("Items"))
            {
                Items.Add(V->AsString());
            }

            //�A�C�e���J�E���g�̎擾
            for (TSharedPtr<FJsonValue> V : JsonRootObject->GetArrayField("ItemCount"))
            {
                Count.Add(V->AsNumber());
            }
            return true;
        }
        return false;
    }
    return false;
}
