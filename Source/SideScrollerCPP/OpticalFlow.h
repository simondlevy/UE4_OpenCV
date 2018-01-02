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

    OpticalFlow(AHUD*hud, int rows, int cols);

    ~OpticalFlow();

	virtual void perform(uint8_t* imagergb) override;

private:

	int _rows;
	int _cols;
	bool _ready;

	AHUD* _hud;

	uint16_t* _imgprev;
	uint16_t* _imgcurr;
};
