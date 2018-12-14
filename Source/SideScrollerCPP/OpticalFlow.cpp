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

#include "ArduEyeOpticalFlow.h"

#include <cstdint>


OpticalFlow::OpticalFlow(int width, int height) : VisionAlgorithm(width, height)
{
}

OpticalFlow::~OpticalFlow()
{
}

void OpticalFlow::perform(void)
{
	// Convert current color image to grayscale
	static cv::Mat prev;
	static bool gotprev;
	cv::Mat curr;
	cv::cvtColor(*_bgrimg, curr, cv::COLOR_BGR2GRAY);

	if (gotprev) {

		Debug::printf("%f %f", cv::mean(curr).val[0], cv::mean(prev).val[0]);
	}

	curr.copyTo(prev);

	gotprev = true;
}

void OpticalFlow::draw(AHUD* hud, int leftx, int topy)
{

}

VisionAlgorithm * getInstance(int width, int height)
{
	return new OpticalFlow(width, height);
}
