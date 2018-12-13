#pragma once

/*
* VisionAlgorithm.cpp: Abstract threaded machine-vision class for UnrealEngine4
*
* Adapted from https://wiki.unrealengine.com/Multi-Threading:_How_to_Create_Threads_in_UE4
*
* Copyright (C) 2017 Simon D. Levy
*
* MIT License
*/

#include "VisionAlgorithm.h"
#include "OnScreenDebug.h"

#include "Engine.h"
#include "Runtime/Core/Public/HAL/Runnable.h"
#include "GameFramework/Actor.h"
#include "OnscreenDebug.h"

#include <opencv2/video/tracking.hpp>
#include <opencv2/core.hpp>

#include <cstdint>

//Thread Worker Starts as NULL, prior to being instanced
//		This line is essential! Compiler error without it
VisionAlgorithm* VisionAlgorithm::Runnable = NULL;

extern VisionAlgorithm* getInstance(int width, int height);

VisionAlgorithm::VisionAlgorithm(int width, int height)
{
	_bgrimg = new cv::Mat(width, height, CV_8UC3);

	Thread = FRunnableThread::Create(this, TEXT("VisionAlgorithm"), 0, TPri_BelowNormal); //windows default = 8mb for thread, could specify more
}

VisionAlgorithm::~VisionAlgorithm()
{
	delete _bgrimg;
}

void VisionAlgorithm::update(cv::Mat & bgrimg)
{
	bgrimg.copyTo(*_bgrimg);
}

//Init
bool VisionAlgorithm::Init()
{
	//Init the Data 

	return true;
}

uint32 VisionAlgorithm::Run()
{
	// Initial wait before starting
	FPlatformProcess::Sleep(0.03);

	// While not told to stop this thread 

	while (StopTaskCounter.GetValue() == 0)
	{
		// Do main work here
		perform();

		// Yield to other threads
		FPlatformProcess::Sleep(0.01);
	}

	// Run VisionAlgorithm::Shutdown() from the timer in Game Thread that is watching
	// to see when VisionAlgorithm::IsThreadFinished()

	return 0;
}

void VisionAlgorithm::Stop()
{
	StopTaskCounter.Increment();
}

VisionAlgorithm* VisionAlgorithm::NewWorker(int width, int height)
{
	//Create new instance of thread if it does not exist and the platform supports multi threading!
	if (!Runnable && FPlatformProcess::SupportsMultithreading()) {

		Runnable = getInstance(width, height);
	}
	return Runnable;
}

void VisionAlgorithm::EnsureCompletion()
{
	Stop();
	Thread->WaitForCompletion();
}

void VisionAlgorithm::Shutdown()
{
	if (Runnable) {

		Runnable->EnsureCompletion();
		delete Runnable;
		Runnable = NULL;
	}
}

