// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveToText.h"

// Load and save text files
bool USaveToText::FileSaveString(FString SaveTextB, FString FileNameB)
{
	return FFileHelper::SaveStringToFile(SaveTextB, *(FileNameB));
}

bool USaveToText::FileLoadString(FString FileNameA, FString& SaveTextA)
{
	return FFileHelper::LoadFileToString(SaveTextA, *(FileNameA));
}

FString USaveToText::FileLoadAndReturnString(FString FileNameA)
{
	FString myString;
	bool myBool = true;
	myBool = FFileHelper::LoadFileToString(myString, *(FileNameA));
	return myString;
}

