#pragma once

/*
* OpticalFlow.cpp: Optical flow for UnrealEngine4
*
* Copyright (C) 2017 Simon D. Levy
*
* MIT License
*/

#include "OpticalFlow.h"

OpticalFlow::OpticalFlow(AHUD* hud, int rows, int cols) : _hud(hud), _rows(rows), _cols(cols)
{
    _imgcurr = new uint16_t [rows*cols];
    _imgprev = new uint16_t [rows*cols];

	_ready = false;
}

OpticalFlow::~OpticalFlow()
{
    delete _imgcurr;
    delete _imgprev;
}

void OpticalFlow::perform(uint8_t* imagergb)
{
    // RGB->gray formula from https ://www.johndcook.com/blog/2009/08/24/algorithms-convert-color-grayscale/
	for (int k = 0; k < _rows*_cols; ++k) {
		_imgcurr[k] = (uint16_t)(0.21 * imagergb[3 * k] + 0.72 * imagergb[3 * k + 1] + 0.07 * imagergb[3 * k + 2]);
	}

	// Fake up optical flow with a line for now
	if (_ready) {

		_hud->Draw2DLine(0, 0, 10, 10, FColor::Yellow);
	}

	_ready = true;

	// Current image becomes previous image on next iteration
	memcpy(_imgprev, _imgcurr, _rows*_cols);
}


