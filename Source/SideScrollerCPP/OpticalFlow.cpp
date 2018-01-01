/*
* OpticalFlow: glue code between RGB image and optical-flow algorithms
*
* Copyright (C) 2017 Simon D. Levy
*
* MIT License
*/

#include "OpticalFlow.h"

OpticalFlow::OpticalFlow(int rows, int cols)
{
	imgprev = new uint16_t[rows*cols];
	imgcurr = new uint16_t[rows*cols];
}

OpticalFlow::~OpticalFlow()
{
	
	delete imgcurr;
	delete imgprev;
}
