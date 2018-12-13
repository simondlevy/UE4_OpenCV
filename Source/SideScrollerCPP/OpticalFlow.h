#pragma once

/*
* OpticalFlow.h: OpenCV demo algorithm for UnrealEngine4
*
* Copyright (C) 2017 Simon D. Levy

* MIT License
*/

#pragma once

#include "VisionAlgorithm.h"

class OpticalFlow  : public VisionAlgorithm {

public:

	OpticalFlow(int width, int height);

    ~OpticalFlow();

	void draw(AHUD* hud, int leftx, int topy) override;


private:

	cv::Mat * _prevgray;
	bool _gotprev;

	// Arbitrary edge-detection params
	const int LOW_THRESHOLD = 50;
	const int RATIO         = 3;
	const int KERNEL_SIZE   = 3;

    // For drawing edges
	FColor EDGE_COLOR = FColor::Yellow;

	// Runs on its own thread
	virtual void perform(void) override;

	// Stores vertices computed on thread
	static const int MAX_VERTICES = 1000;
	cv::Point _vertices[MAX_VERTICES];
	int _vertexCount;

	// Helper
	static int min(int a, int b) { return a < b ? a : b; }
};
