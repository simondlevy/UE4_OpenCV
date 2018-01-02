#pragma once

/*
* OpticalFlow.h: Optical flow for UnrealEngine4
*
* Copyright (C) 2017 Simon D. Levy
*
* MIT License
*/

#include "VisionAlgorithm.h"

class OpticalFlow : public VisionAlgorithm {

public:

    OpticalFlow(int rows, int cols);

    ~OpticalFlow();

	virtual void perform(uint8_t* imagergb, UTextureRenderTarget2D* textureRenderTarget) override;

private:

	uint16_t * imgprev;
	uint16_t * imgcurr;
};
