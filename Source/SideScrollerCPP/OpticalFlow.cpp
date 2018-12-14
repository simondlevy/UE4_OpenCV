#pragma once

/*
* OpticalFlow.cpp: OpenCV demo algorithm for UnrealEngine4
*
* Copyright (C) 2017 Simon D. Levy
*
* MIT License
*/

#include "OpticalFlow.h"
#include "OnScreenDebug.h"

#include "Engine.h"
#include "Runtime/Core/Public/HAL/Runnable.h"
#include "GameFramework/Actor.h"
#include "OnscreenDebug.h"

#include <opencv2/video/tracking.hpp>
#include <opencv2/core.hpp>

#include <cstdint>


OpticalFlow::OpticalFlow(int width, int height) : VisionAlgorithm(width, height)
{
	_vertexCount = 0;
	_prevgray = new cv::Mat(height, width, CV_8UC3);
	_gotprev = false;
}

OpticalFlow::~OpticalFlow()
{
	delete _prevgray;
}

void OpticalFlow::perform(void)
{
	// Convert color image to grayscale
	cv::Mat gray;
	cv::cvtColor(*_bgrimg, gray, cv::COLOR_BGR2GRAY);

	if (_gotprev) {

		for (int j = 0; j < gray.rows; ++j) {
			for (int k = 0; k < gray.cols; ++k) {
			}
		}
	}

	gray.copyTo(*_prevgray);
	_gotprev = true;
}

void OpticalFlow::draw(AHUD* hud, int leftx, int topy) 
{ 

}

VisionAlgorithm * getInstance(int width, int height)
{
	return new OpticalFlow(width, height);
}
