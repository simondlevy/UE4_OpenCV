#pragma once

/*
* OpticalFlow.cpp: Optical flow for UnrealEngine4
*
* Copyright (C) 2017 Simon D. Levy
*
* MIT License
*/

#include "OpticalFlow.h"
#include "OnscreenDebug.h"

#include <opencv2/core.hpp>
#include <opencv2/video/tracking.hpp>


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


	// Run optical flow after we have a second frame
	if (_ready) {

		cv::Mat curr(_rows, _cols, CV_8UC1, _imgcurr);
		cv::Mat prev(_rows, _cols, CV_8UC1, _imgprev);

		cv::Mat flow;
		cv::calcOpticalFlowFarneback(prev, curr, flow, 0.5, 3, 15, 3, 5, 1.2, 0);
	
		int skip = 16;
		float div = 5;

		for (int x = 0; x < _cols; x+=skip) {
			for (int y = 0; y < _rows; y+= skip) {

				float fx = flow.ptr<float>(y, x)[0] / div;
				float fy = flow.ptr<float>(y, x)[1] / div;

				int cx = _leftx + x;
				int cy = _topy + y;

				_hud->Draw2DLine(cx, cy, cx+fx, cy+fy, FColor::Yellow);
			}
		}
	}

	_ready = true;

	// Current image becomes previous image on next iteration
	memcpy(_imgprev, _imgcurr, _rows*_cols);
}
