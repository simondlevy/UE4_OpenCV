/*
* OpticalFlow: glue code between RGB image and optical-flow algorithms
*
* Copyright (C) 2017 Simon D. Levy
*
* MIT License
*/

#pragma once

#include <cstdint>

/**
 * 
 */
class OpticalFlow
{
public:

	OpticalFlow(int rows, int cols);

	~OpticalFlow();

private:

	uint16_t* imgcurr;
	uint16_t* imgprev;
};
