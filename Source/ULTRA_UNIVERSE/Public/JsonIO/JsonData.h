// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "JsonData.generated.h"

UENUM(BlueprintType)
enum class EJsonDataType :uint8
{
	//初期値
	Null,
	//データが格納されていることを示す
	Data,
	//配列が格納されていることを示す
	Array,
	//マップ(タグとデータがセットで登録されている配列)が格納されていることを示す
	Map,
};

//----------------------------------------------------------------------------------------------------------------------------------------
//****************************************************************************************************************************************
//----------------------------------------------------------------------------------------------------------------------------------------

UCLASS( ClassGroup=(Custom), meta=(ChildCannotTick) )
class ULTRA_UNIVERSE_API AJsonDataObject : public AActor
{
	GENERATED_BODY()

public:
	//何のデータが格納されているか確認するための変数
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DataType")
		EJsonDataType m_type;

	//データ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		FString m_data;

	//配列
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DataArray")
		TArray<AJsonDataObject*> m_array;

	//マップ(タグとデータがセットで登録されている配列)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DataMap")
		TMap<FString, AJsonDataObject*> m_map;

	//親
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DataParent")
		AActor* m_parent;

public:	
	// Sets default values for this component's properties
	AJsonDataObject();

public:
	//データの消去
	UFUNCTION(BlueprintCallable, Category = "JsonDataObject")
		void Erase();

	//複製を行う
	UFUNCTION(BlueprintCallable, Category = "JsonDataObject")
		AJsonDataObject* Clone(AJsonDataObject* parent = nullptr);

	//----------------------------------------------------------------------------------------------------------------------------------------

	//配列・マップから指定したJsonDataObjectを外す
	UFUNCTION(BlueprintCallable, Category = "JsonDataObject")
		bool RemoveHaveData(AJsonDataObject* child);

	//配列・マップから指定したJsonDataObjectを削除する
	UFUNCTION(BlueprintCallable, Category = "JsonDataObject")
		bool EraseHaveData(AJsonDataObject* child);

	//配列・マップから指定したJsonDataObjectを移動先のJsonDataObjectに移動する
	UFUNCTION(BlueprintCallable, Category = "JsonDataObject")
		bool MoveHaveData(AJsonDataObject* child, AJsonDataObject* destJsonDataObj,const FString& moveDataAfterTag = "");

	//----------------------------------------------------------------------------------------------------------------------------------------

	//指定した配列のデータを移動先の配列に移動させる
	UFUNCTION(BlueprintCallable, Category = "JsonDataObject:Array")
		bool MoveDataArrayToArray(AJsonDataObject* moveData, AJsonDataObject* destJsonDataObj);
	UFUNCTION(BlueprintCallable, Category = "JsonDataObject:Array")
		bool MoveDataArrayToMap(AJsonDataObject* moveData, AJsonDataObject* destJsonDataObj, const FString& moveDataAfterTag);

	//----------------------------------------------------------------------------------------------------------------------------------------

	//マップのタグを変化する
	UFUNCTION(BlueprintCallable, Category = "JsonDataObject:Map")
		bool ChangeMapTagDataObj(AJsonDataObject* child,const FString& afterTag);
	UFUNCTION(BlueprintCallable, Category = "JsonDataObject:Map")
		bool ChangeMapTagDataTag(const FString& beforeTag, const FString& afterTag);

	//指定したマップのタグのデータを消去する
	UFUNCTION(BlueprintCallable, Category = "JsonDataObject:Map")
		bool EraseMapTagData(const FString& eraseDataTag);

	//指定したマップのタグのデータを移動先の配列に移動させる
	UFUNCTION(BlueprintCallable, Category = "JsonDataObject:Map")
		bool MoveTagDataMapToMap(const FString& moveDataTag, AJsonDataObject* destJsonDataObj, const FString& moveDataAfterTag);
	UFUNCTION(BlueprintCallable, Category = "JsonDataObject:Map")
		bool MoveTagDataMapToArray(const FString& moveDataTag, AJsonDataObject* destJsonDataObj);

	//指定したマップのデータを移動先の配列に移動させる
	UFUNCTION(BlueprintCallable, Category = "JsonDataObject:Map")
		bool MoveDataMapToMap(AJsonDataObject* moveData, AJsonDataObject* destJsonDataObj, const FString& moveDataAfterTag);
	UFUNCTION(BlueprintCallable, Category = "JsonDataObject:Map")
		bool MoveDataMapToArray(AJsonDataObject* moveData, AJsonDataObject* destJsonDataObj);

	//----------------------------------------------------------------------------------------------------------------------------------------

private:
	AJsonDataObject* CreateCloneObject(AJsonDataObject* parent = nullptr);

};
