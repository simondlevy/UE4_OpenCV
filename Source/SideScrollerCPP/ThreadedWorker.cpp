#pragma once

/*
* ThreadedWorker.cpp: Code for running compute-intensive computations on their own thread
*
* Adapted from https://wiki.unrealengine.com/Multi-Threading:_How_to_Create_Threads_in_UE4
*
* Copyright (C) 2018 Simon D. Levy
*
* MIT License
*/

#include "ThreadedWorker.h"
#include "Runtime/Core/Public/GenericPlatform/GenericPlatformAffinity.h"

//***********************************************************
//Thread Worker Starts as NULL, prior to being instanced
//		This line is essential! Compiler error without it
FThreadedWorker* FThreadedWorker::Runnable = NULL;
//***********************************************************


FThreadedWorker::FThreadedWorker(void) 
{
	Thread = FRunnableThread::Create(this, TEXT("FThreadedWorker"), 0, TPri_BelowNormal); //windows default = 8mb for thread, could specify more
}

FThreadedWorker::~FThreadedWorker()
{
	delete Thread;
	Thread = NULL;
}

//Init
bool FThreadedWorker::Init()
{
	//Init the Data 

	return true;
}

//Run
uint32 FThreadedWorker::Run()
{
	//Initial wait before starting
	FPlatformProcess::Sleep(0.03);

	//While not told to stop this thread 
	//		and not yet finished finding Prime Numbers
	while (StopTaskCounter.GetValue() == 0)
	{
		// Do main work here

		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		//prevent thread from using too many resources
		//FPlatformProcess::Sleep(0.01);
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	}

	//Run FThreadedWorker::Shutdown() from the timer in Game Thread that is watching
	//to see when FThreadedWorker::IsThreadFinished()

	return 0;
}

//stop
void FThreadedWorker::Stop()
{
	StopTaskCounter.Increment();
}

FThreadedWorker* FThreadedWorker::NewWorker(void)
{
	//Create new instance of thread if it does not exist
	//		and the platform supports multi threading!
	if (!Runnable && FPlatformProcess::SupportsMultithreading())
	{
		Runnable = new FThreadedWorker();
	}
	return Runnable;
}

void FThreadedWorker::EnsureCompletion()
{
	Stop();
	Thread->WaitForCompletion();
}

void FThreadedWorker::Shutdown()
{
	if (Runnable)
	{
		Runnable->EnsureCompletion();
		delete Runnable;
		Runnable = NULL;
	}
}



