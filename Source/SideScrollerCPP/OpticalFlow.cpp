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

		int16_t ofx = 0;
		int16_t ofy = 0;
		ofoLK_Plus_2D(_imgcurr, _imgprev, _rows, _cols, 100, &ofx, &ofy);
		int ctrx = _leftx + _cols / 2;
		int ctry = _topy  + _rows / 2;
		_hud->Draw2DLine(ctrx, ctry, ctrx+ofx, ctry+ofy, FColor::Yellow);
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

	// Compute final output. Note use of "scale" here to multiply 2*top   
	// to a larger number so that it may be meaningfully divided using 
	// fixed point arithmetic
	int64_t XS = ((int64_t)(b1)*A22 - (int64_t)(b2)*A12) * scale / detA;
	int64_t YS = ((int64_t)(b2)*A11 - (int64_t)(b1)*A12) * scale / detA;

	(*ofx) = (int16_t)XS;
	(*ofy) = (int16_t)YS;
}



