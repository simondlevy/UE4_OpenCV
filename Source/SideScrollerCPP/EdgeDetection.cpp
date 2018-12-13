#pragma once

/*
* EdgeDetection.cpp: OpenCV demo algorithm for UnrealEngine4
*
* Copyright (C) 2017 Simon D. Levy
*
* MIT License
*/

#include "EdgeDetection.h"
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
EdgeDetection* EdgeDetection::Runnable = NULL;

EdgeDetection::EdgeDetection(int width, int height)
{
	_bgrimg = new cv::Mat(width, height, CV_8UC3);

	_vertexCount = 0;

	Thread = FRunnableThread::Create(this, TEXT("EdgeDetection"), 0, TPri_BelowNormal); //windows default = 8mb for thread, could specify more
}

EdgeDetection::~EdgeDetection()
{
	delete _bgrimg;
}

void EdgeDetection::perform(void)
{
	// Convert color image to grayscale
	cv::Mat gray;
	cv::cvtColor(*_bgrimg, gray, cv::COLOR_BGR2GRAY);

	// Reduce noise with a kernel 3x3
	cv::Mat detected_edges;
	cv::blur(gray, detected_edges, cv::Size(3, 3));

	// Run Canny edge detection algorithm on blurred gray image
	cv::Canny(detected_edges, detected_edges, LOW_THRESHOLD, LOW_THRESHOLD*RATIO, KERNEL_SIZE);

	// Find edges
	cv::Mat nonZeroCoordinates;
	cv::findNonZero(detected_edges, nonZeroCoordinates);

	// Store vertices (edge coordinates)
	_vertexCount = min((int)nonZeroCoordinates.total(), MAX_VERTICES);
	for (int i = 0; i < _vertexCount; i++) {
		cv::Point vertex = nonZeroCoordinates.at<cv::Point>(i);
		_vertices[i].x = vertex.x;
		_vertices[i].y = vertex.y;
	}
}

void EdgeDetection::update(cv::Mat & bgrimg)
{
	bgrimg.copyTo(*_bgrimg);
}

void EdgeDetection::draw(AHUD* hud, int leftx, int topy) 
{ 
	for (int i = 0; i < _vertexCount; ++i) {
		cv::Point vertex = _vertices[i];
		hud->DrawRect(EDGE_COLOR, leftx + vertex.x, topy + vertex.y, 1, 1);
	}
}

//Init
bool EdgeDetection::Init()
{
	//Init the Data 

	return true;
}

uint32 EdgeDetection::Run()
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

	// Run EdgeDetection::Shutdown() from the timer in Game Thread that is watching
	// to see when EdgeDetection::IsThreadFinished()

	return 0;
}

void EdgeDetection::Stop()
{
	StopTaskCounter.Increment();
}

EdgeDetection* EdgeDetection::NewWorker(int width, int height)
{
	//Create new instance of thread if it does not exist
	//		and the platform supports multi threading!
	if (!Runnable && FPlatformProcess::SupportsMultithreading()) {

		Runnable = new EdgeDetection(width, height);
	}
	return Runnable;
}

void EdgeDetection::EnsureCompletion()
{
	Stop();
	Thread->WaitForCompletion();
}

void EdgeDetection::Shutdown()
{
	if (Runnable) {

		Runnable->EnsureCompletion();
		delete Runnable;
		Runnable = NULL;
	}
}

