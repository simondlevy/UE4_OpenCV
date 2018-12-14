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

    // For drawing flow arrows
	FColor ARROW_COLOR = FColor::Green;

	// Runs on its own thread
	virtual void perform(void) override;
};
