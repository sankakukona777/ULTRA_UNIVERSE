// Fill out your copyright notice in the Description page of Project Settings.


#include "JsonIO/JsonIO.h"

#include "Misc/Paths.h"
#include "Misc/FileHelper.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"

bool UJsonIO::SaveJsonData(const FString& savePath,FString CharName, TArray<AActor*> Items, TArray<int> Count)
{
    JsonObjectPtr JsonRootObject = MakeShareable(new FJsonObject);

    //保存した日時
    const FString SaveDate = FDateTime::UtcNow().ToString();
    JsonRootObject->SetStringField("SaveDate", SaveDate);

    //キャラ名
    JsonRootObject->SetStringField("Character", CharName);

    //レベル上のオブジェクトの名前
    TArray<TSharedPtr<FJsonValue>> ItemObjects;

    for (AActor* obj : Items)
    {
        ItemObjects.Add(MakeShareable(new FJsonValueString(obj->GetFullName())));
    }
    JsonRootObject->SetArrayField("Items", ItemObjects);

    //数値の配列
    TArray<TSharedPtr<FJsonValue>> ItemCountArray;
    for (int i : Count)
    {
        ItemCountArray.Add(MakeShareable(new FJsonValueNumber(i)));
    }
    JsonRootObject->SetArrayField("ItemCount", ItemCountArray);

    // FStringにJsonを書き込むためのWriterを作成
    FString OutPutString;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutPutString);

    // JsonをFStringに書き込み
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
        // FJsonObject(Jsonデータの入れ物)を作成
        JsonObjectPtr JsonRootObject = MakeShareable(new FJsonObject());

        // FStringからJsonを読み込むためのReaderを作成
        TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(RawData);

        // Json文字列からJsonオブジェクトに読み込み
        if (FJsonSerializer::Deserialize(JsonReader, JsonRootObject))
        {
            //キャラネームの取得
            CharName = JsonRootObject->GetStringField("Character");

            //レベル上のオブジェクトの取得
            for (TSharedPtr<FJsonValue> V : JsonRootObject->GetArrayField("Items"))
            {
                Items.Add(V->AsString());
            }

            //アイテムカウントの取得
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
