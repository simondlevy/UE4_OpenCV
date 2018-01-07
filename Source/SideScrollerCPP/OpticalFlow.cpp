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

		/*
		int16_t ofx = 0;
		int16_t ofy = 0;
		ofoLK_Plus_2D(_imgcurr, _imgprev, _rows, _cols, 100, &ofx, &ofy);

		_hud->Draw2DLine(ctrx, ctry, ctrx+ofx, ctry+ofy, FColor::Yellow);
		*/
	}

	_ready = true;

	// Current image becomes previous image on next iteration
	memcpy(_imgprev, _imgcurr, _rows*_cols);
}

// Adapted from https://github.com/simondlevy/ArduEye/blob/master/src/OpticalFlow.cpp
void OpticalFlow::ofoLK_Plus_2D(uint8_t *curr_img, uint8_t *last_img, uint16_t rows, uint16_t cols, uint16_t scale, int16_t * ofx, int16_t * ofy)
{
	int32_t  A11 = 0, A12 = 0, A22 = 0, b1 = 0, b2 = 0;
	int16_t  F2F1, F4F3, FCF0;

	// set up pointers
	uint8_t *f0 = curr_img + cols + 1; //center image
	uint8_t *f1 = curr_img + cols + 2; //right-shifted image
	uint8_t *f2 = curr_img + cols;     //left-shifted image	
	uint8_t *f3 = curr_img + 2 * cols + 1; //down-shifted image	
	uint8_t *f4 = curr_img + 1;		//up-shifted image
	uint8_t *fz = last_img + cols + 1; 	//time-shifted image

										// loop through
	for (uint8_t r = 1; r<rows - 1; ++r) {

		for (uint8_t c = 1; c<cols - 1; ++c) {

			// compute differentials, then increment pointers (post 		// increment)
			F2F1 = (*(f2++) - *(f1++));	//horizontal differential
			F4F3 = (*(f4++) - *(f3++));	//vertical differential
			FCF0 = (*(fz++) - *(f0++));	//time differential

										// update summations
			A11 += (F2F1 * F2F1);
			A12 += (F4F3 * F2F1);
			A22 += (F4F3 * F4F3);
			b1 += (FCF0 * F2F1);
			b2 += (FCF0 * F4F3);
		}
		f0 += 2;	//move to next row of image
		fz += 2;
		f1 += 2;
		f2 += 2;
		f3 += 2;
		f4 += 2;
	}

	//determinant
	int64_t detA = ((int64_t)(A11)*A22 - (int64_t)(A12)*A12);

	int64_t XS = 0;
	int64_t YS = 0;

	// Compute final output. Note use of "scale" here to multiply 2*top   
	// to a larger number so that it may be meaningfully divided using 
	// fixed point arithmetic	
	if (detA != 0) {
		XS = ((int64_t)(b1)*A22 - (int64_t)(b2)*A12) * scale / detA;
		YS = ((int64_t)(b2)*A11 - (int64_t)(b1)*A12) * scale / detA;
	}

	(*ofx) = (int16_t)XS;
	(*ofy) = (int16_t)YS;
}

