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


EdgeDetection::EdgeDetection(int width, int height) : VisionAlgorithm(width, height)
{
	_vertexCount = 0;
}

EdgeDetection::~EdgeDetection()
{
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

void EdgeDetection::draw(AHUD* hud, int leftx, int topy) 
{ 
	for (int i = 0; i < _vertexCount; ++i) {
		cv::Point vertex = _vertices[i];
		hud->DrawRect(EDGE_COLOR, leftx + vertex.x, topy + vertex.y, 1, 1);
	}
}

VisionAlgorithm * getInstance(int width, int height)
{
	return new EdgeDetection(width, height);
}
