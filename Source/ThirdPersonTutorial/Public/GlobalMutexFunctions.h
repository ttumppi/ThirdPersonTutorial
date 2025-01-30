// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "mutex"

/**
 * 
 */
static class THIRDPERSONTUTORIAL_API GlobalMutexFunctions
{
public:

	template<typename T>
	static void SetValueThreadSafely(std::mutex& lock, T& valueToAssignTo, T& valueToAssign) {
		lock.lock();
		valueToAssignTo = valueToAssign;
		lock.unlock();
	}

};
