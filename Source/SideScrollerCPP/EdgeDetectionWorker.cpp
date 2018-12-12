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
FEdgeDetectionWorker* FEdgeDetectionWorker::Runnable = NULL;
//***********************************************************


FEdgeDetectionWorker::FEdgeDetectionWorker(TArray<uint32>& TheArray, const int32 IN_TotalPrimesToFind) :
	TotalPrimesToFind(IN_TotalPrimesToFind)
	, StopTaskCounter(0)
	, PrimesFoundCount(0)
{
	//Link to where data should be stored
	EdgeDetections = &TheArray;

	Thread = FRunnableThread::Create(this, TEXT("FEdgeDetectionWorker"), 0, TPri_BelowNormal); //windows default = 8mb for thread, could specify more
}

FEdgeDetectionWorker::~FEdgeDetectionWorker()
{
	delete Thread;
	Thread = NULL;
}

//Init
bool FEdgeDetectionWorker::Init()
{
	//Init the Data 

	return true;
}

//Run
uint32 FEdgeDetectionWorker::Run()
{
	//Initial wait before starting
	FPlatformProcess::Sleep(0.03);

	//While not told to stop this thread 
	//		and not yet finished finding Prime Numbers
	while (StopTaskCounter.GetValue() == 0 && !IsFinished())
	{
		//EdgeDetections->Add(FindNextEdgeDetection());
		//PrimesFoundCount++;

		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		//prevent thread from using too many resources
		//FPlatformProcess::Sleep(0.01);
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	}

	//Run FEdgeDetectionWorker::Shutdown() from the timer in Game Thread that is watching
	//to see when FEdgeDetectionWorker::IsThreadFinished()

	return 0;
}

//stop
void FEdgeDetectionWorker::Stop()
{
	StopTaskCounter.Increment();
}

FEdgeDetectionWorker* FEdgeDetectionWorker::JoyInit(TArray<uint32>& TheArray, const int32 IN_TotalPrimesToFind)
{
	//Create new instance of thread if it does not exist
	//		and the platform supports multi threading!
	if (!Runnable && FPlatformProcess::SupportsMultithreading())
	{
		Runnable = new FEdgeDetectionWorker(TheArray, IN_TotalPrimesToFind);
	}
	return Runnable;
}

void FEdgeDetectionWorker::EnsureCompletion()
{
	Stop();
	Thread->WaitForCompletion();
}

void FEdgeDetectionWorker::Shutdown()
{
	if (Runnable)
	{
		Runnable->EnsureCompletion();
		delete Runnable;
		Runnable = NULL;
	}
}

bool FEdgeDetectionWorker::IsThreadFinished()
{
	if (Runnable) return Runnable->IsFinished();
	return true;
}


