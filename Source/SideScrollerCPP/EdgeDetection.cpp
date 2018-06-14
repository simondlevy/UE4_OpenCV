#pragma once

/*
* EdgeDetection.cpp: OpenCV demo algorithm for UnrealEngine4
*
* Copyright (C) 2017 Simon D. Levy
*
* MIT License
*/

#include "EdgeDetection.h"
#include "OnscreenDebug.h"

#include <opencv2/core.hpp>
#include <opencv2/video/tracking.hpp>

EdgeDetection::EdgeDetection(AHUD* hud, int leftx, int topy, int rows, int cols) : 
	_leftx(leftx), _topy(topy), _hud(hud),  _rows(rows), _cols(cols)
{
}

EdgeDetection::~EdgeDetection()
{
}

void EdgeDetection::perform(uint8_t* imagergb)
{
    // RGB->gray formula from https ://www.johndcook.com/blog/2009/08/24/algorithms-convert-color-grayscale/
	cv::Mat gray(_rows, _cols, CV_8UC1);
	for (int k = 0; k < _rows*_cols; ++k) {
		gray.at<uint8_t>(k) = (uint8_t)(0.21 * imagergb[3 * k] + 0.72 * imagergb[3 * k + 1] + 0.07 * imagergb[3 * k + 2]);
	}

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
