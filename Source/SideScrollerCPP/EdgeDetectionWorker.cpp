#pragma once

/*
* EdgeDetectionWorker.cpp: Code for running OpenCV edge detection on its own thread
*
* Adapted from https://wiki.unrealengine.com/Multi-Threading:_How_to_Create_Threads_in_UE4
*
* Copyright (C) 2018 Simon D. Levy
*
* MIT License
*/

#include "EdgeDetectionWorker.h"
#include "Runtime/Core/Public/GenericPlatform/GenericPlatformAffinity.h"
#include <stdlib.h>

//***********************************************************
//Thread Worker Starts as NULL, prior to being instanced
//		This line is essential! Compiler error without it
FPrimeNumberWorker* FPrimeNumberWorker::Runnable = NULL;
//***********************************************************


FPrimeNumberWorker::FPrimeNumberWorker(TArray<uint32>& TheArray, const int32 IN_TotalPrimesToFind) :
	TotalPrimesToFind(IN_TotalPrimesToFind)
	, StopTaskCounter(0)
	, PrimesFoundCount(0)
{
	//Link to where data should be stored
	PrimeNumbers = &TheArray;

	Thread = FRunnableThread::Create(this, TEXT("FPrimeNumberWorker"), 0, TPri_BelowNormal); //windows default = 8mb for thread, could specify more
}

FPrimeNumberWorker::~FPrimeNumberWorker()
{
	delete Thread;
	Thread = NULL;
}

//Init
bool FPrimeNumberWorker::Init()
{
	//Init the Data 
	PrimeNumbers->Empty();
	PrimeNumbers->Add(2);
	PrimeNumbers->Add(3);


	return true;
}

//Run
uint32 FPrimeNumberWorker::Run()
{
	//Initial wait before starting
	FPlatformProcess::Sleep(0.03);

	//While not told to stop this thread 
	//		and not yet finished finding Prime Numbers
	while (StopTaskCounter.GetValue() == 0 && !IsFinished())
	{
		//PrimeNumbers->Add(FindNextPrimeNumber());
		//PrimesFoundCount++;

		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		//prevent thread from using too many resources
		//FPlatformProcess::Sleep(0.01);
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	}

	//Run FPrimeNumberWorker::Shutdown() from the timer in Game Thread that is watching
	//to see when FPrimeNumberWorker::IsThreadFinished()

	return 0;
}

//stop
void FPrimeNumberWorker::Stop()
{
	StopTaskCounter.Increment();
}

FPrimeNumberWorker* FPrimeNumberWorker::JoyInit(TArray<uint32>& TheArray, const int32 IN_TotalPrimesToFind)
{
	//Create new instance of thread if it does not exist
	//		and the platform supports multi threading!
	if (!Runnable && FPlatformProcess::SupportsMultithreading())
	{
		Runnable = new FPrimeNumberWorker(TheArray, IN_TotalPrimesToFind);
	}
	return Runnable;
}

void FPrimeNumberWorker::EnsureCompletion()
{
	Stop();
	Thread->WaitForCompletion();
}

void FPrimeNumberWorker::Shutdown()
{
	if (Runnable)
	{
		Runnable->EnsureCompletion();
		delete Runnable;
		Runnable = NULL;
	}
}

bool FPrimeNumberWorker::IsThreadFinished()
{
	if (Runnable) return Runnable->IsFinished();
	return true;
}
int32 FPrimeNumberWorker::FindNextPrimeNumber()
{
	//Last known prime number  + 1
	int32 TestPrime = PrimeNumbers->Last();

	bool NumIsPrime = false;
	while (!NumIsPrime)
	{
		NumIsPrime = true;

		//Try Next Number
		TestPrime++;

		//Modulus from 2 to current number - 1 
		for (int32 b = 2; b < TestPrime; b++)
		{
			//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
			//prevent thread from using too many resources
			//FPlatformProcess::Sleep(0.01);
			//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

			if (TestPrime % b == 0)
			{
				NumIsPrime = false;
				break;
				//~~~
			}
		}
	}

	//Success!
	return TestPrime;
}

