#pragma once

/*
* OpticalFlow.cpp: Optical flow for UnrealEngine4
*
* Copyright (C) 2017 Simon D. Levy
*
* MIT License
*/

#include "OpticalFlow.h"

OpticalFlow::OpticalFlow(int rows, int cols)
{
    imgcurr = new uint16_t [rows*cols];
    imgprev = new uint16_t [rows*cols];
}

OpticalFlow::~OpticalFlow()
{
    delete imgcurr;
    delete imgprev;
}

void OpticalFlow::perform(uint8_t* imagergb, UTextureRenderTarget2D* textureRenderTarget)
{
    // RGB->gray formula from https ://www.johndcook.com/blog/2009/08/24/algorithms-convert-color-grayscale/
    //imgdata[k] = (byte)(0.21 *PixelColor.R + 0.72 * PixelColor.G + 0.07 * PixelColor.B);
}


