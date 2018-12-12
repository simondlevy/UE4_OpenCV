#pragma once

/*
* Threaded.cpp: OpenCV demo algorithm for UnrealEngine4
*
* Copyright (C) 2017 Simon D. Levy
*
* MIT License
*/

#include "EdgeDetection.h"
#include "OnscreenDebug.h"

#include <opencv2/video/tracking.hpp>

EdgeDetection::EdgeDetection(AHUD* hud, int leftx, int topy) : VisionAlgorithm(hud, leftx, topy)
{
}

EdgeDetection::~EdgeDetection()
{
}

void EdgeDetection::perform(cv::Mat & bgrimg)
{
    // Convert color image to grayscale
    cv::Mat gray;
    cv::cvtColor(bgrimg, gray, cv::COLOR_BGR2GRAY);

	// Reduce noise with a kernel 3x3
	cv::Mat detected_edges;
	cv::blur(gray, detected_edges, cv::Size(3, 3));

	// Run Canny edge detection algorithm on blurred gray image
	cv::Canny(detected_edges, detected_edges, LOW_THRESHOLD, LOW_THRESHOLD*RATIO, KERNEL_SIZE);

	// Draw edges to HUD
	cv::Mat nonZeroCoordinates;
	cv::findNonZero(detected_edges, nonZeroCoordinates);
	for (int i = 0; i < (int)nonZeroCoordinates.total(); i++) {
		cv::Point point = nonZeroCoordinates.at<cv::Point>(i);
		int x = _leftx + point.x;
		int y = _topy + point.y;
		_hud->DrawRect(EDGE_COLOR, x, y, 1, 1);
	}
}
