// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "JsonData.generated.h"

UENUM(BlueprintType)
enum class EJsonDataType :uint8
{
	//�����l
	Null,
	//�f�[�^���i�[����Ă��邱�Ƃ�����
	Data,
	//�z�񂪊i�[����Ă��邱�Ƃ�����
	Array,
	//�}�b�v(�^�O�ƃf�[�^���Z�b�g�œo�^����Ă���z��)���i�[����Ă��邱�Ƃ�����
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
	//���̃f�[�^���i�[����Ă��邩�m�F���邽�߂̕ϐ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DataType")
		EJsonDataType m_type;

	//�f�[�^
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		FString m_data;

	//�z��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DataArray")
		TArray<AJsonDataObject*> m_array;

	//�}�b�v(�^�O�ƃf�[�^���Z�b�g�œo�^����Ă���z��)
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "DataMap")
		TMap<FString, AJsonDataObject*> m_map;

	//�e
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DataParent")
		AActor* m_parent;

public:	
	// Sets default values for this component's properties
	AJsonDataObject();

public:
	//�������s��
	UFUNCTION(BlueprintCallable, Category = "JsonDataObject")
		AJsonDataObject* Clone(AJsonDataObject* parent = nullptr);

private:
	AJsonDataObject* CreateCloneObject(AJsonDataObject* parent = nullptr);

};
