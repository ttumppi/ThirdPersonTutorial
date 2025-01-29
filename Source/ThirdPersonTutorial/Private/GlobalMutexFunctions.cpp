// Fill out your copyright notice in the Description page of Project Settings.


#include "GlobalMutexFunctions.h"

template<typename T>
static void GlobalMutexFunctions::SetValueThreadSafely(std::mutex& lock, T& valueToAssignTo, T& valueToAssign) {
	lock.lock();
	valueToAssignTo = valueToAssign;
	lock.unlock();
}
