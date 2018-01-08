#pragma once

/*
* VisionAlgorithm.h: Abstract machine-vision class for UnrealEngine4
*
* Copyright (C) 2017 Simon D. Levy
*
* MIT License
*/

#include "Engine.h"

#include <cstdint>

/**
*  Abstract machine-vision class.
*/
class VisionAlgorithm {

public:

	/**
	 * Mandatory virtual destructor to avoid compiler errors.
	*/
	virtual ~VisionAlgorithm() { }

	/**
	* Your implementing class should provide this method.
	* @param imagergb RGB image in row-major order. Each pixel is Red byte followed by Blue byte followed by Green byte.
	*/
	virtual void perform(uint8_t* imagergb) = 0;
};

