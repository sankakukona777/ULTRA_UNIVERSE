// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Dom/JsonObject.h"

#include "JsonIO.generated.h"

/**
 * 
 */

typedef TSharedPtr<FJsonObject> JsonObjectPtr;

UCLASS()
class ULTRA_UNIVERSE_API UJsonIO : public UBlueprintFunctionLibrary
{

	GENERATED_BODY()
	
    UFUNCTION(BlueprintCallable, Category = "Json")
        static bool SaveJsonData(const FString& savePath ,FString CharName, TArray<AActor*> Items, TArray<int> Count);

    UFUNCTION(BlueprintCallable, Category = "Json")
        static bool LoadJsonData(const FString& loadPath, FString& CharName, TArray<FString>& Items, TArray<int>& Count);
};
