// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Dom/JsonObject.h"
#include "JsonData.h"

#include "JsonIO.generated.h"

/**
 * 
 */

UCLASS()
class ULTRA_UNIVERSE_API UJsonIO : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()
	
    UFUNCTION(BlueprintCallable, Category = "JsonIO")
        static bool SaveJsonData(const FString& savePath , AJsonDataObject* data);

    UFUNCTION(BlueprintCallable, Category = "JsonIO")
        static bool LoadJsonData(const FString& loadPath,const FString& loadJsonDataName,AActor* act,AJsonDataObject*& data);

    //----------------------------------------------------------------------------------------------------------------------------------------
    //****************************************************************************************************************************************
    //----------------------------------------------------------------------------------------------------------------------------------------

    static AJsonDataObject* CreateJsonDataObjectFromFJsonObject(const FString& jsonDataName, const TSharedPtr<FJsonObject>& jsonData, AActor* parent);
    static AJsonDataObject* CreateJsonDataObject(const FString& jsonDataName, FJsonValue* jsonData, AActor* parent);
    static void SetJsonDataArrayObject(const FString& jsonDataName, AJsonDataObject* jsonDataObject, const TArray<TSharedPtr<FJsonValue>>& jsonDatas);
    static void SetJsonDataObjectObject(const FString& jsonDataName, AJsonDataObject* jsonDataObject, const TSharedPtr<FJsonObject>& jsonDatas);

    //----------------------------------------------------------------------------------------------------------------------------------------
    //****************************************************************************************************************************************
    //----------------------------------------------------------------------------------------------------------------------------------------

    static bool CreateSaveJsonData(AJsonDataObject* data, TSharedPtr<FJsonValue>& value);
};
