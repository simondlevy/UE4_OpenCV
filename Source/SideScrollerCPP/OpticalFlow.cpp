#pragma once

/*
* OpticalFlow.cpp: Optical flow for UnrealEngine4
*
* Copyright (C) 2017 Simon D. Levy
*
* MIT License
*/

#include "OpticalFlow.h"

OpticalFlow::OpticalFlow(AHUD* hud, int leftx, int topy, int rows, int cols) : 
	_leftx(leftx), _topy(topy), _hud(hud),  _rows(rows), _cols(cols)
{
    _imgcurr = new uint8_t [rows*cols];
    _imgprev = new uint8_t [rows*cols];

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
		_imgcurr[k] = (uint8_t)(0.21 * imagergb[3 * k] + 0.72 * imagergb[3 * k + 1] + 0.07 * imagergb[3 * k + 2]);
	}

	// Fake up optical flow with a line for now
	if (_ready) {

		_hud->Draw2DLine(_leftx, _topy, _leftx+20, _topy+20, FColor::Yellow);
	}

	_ready = true;

	// Current image becomes previous image on next iteration
	memcpy(_imgprev, _imgcurr, _rows*_cols);
}


