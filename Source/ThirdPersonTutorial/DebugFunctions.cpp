// Fill out your copyright notice in the Description page of Project Settings.


#include "DebugFunctions.h"

void DebugFunctions::PrintMessage(int id, FColor color, const std::string text) {
	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(id, 5.0f, color, FString(text.c_str()));
	}
}