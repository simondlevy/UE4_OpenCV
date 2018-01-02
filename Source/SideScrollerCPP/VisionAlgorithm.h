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

class VisionAlgorithm {

public:

	virtual ~VisionAlgorithm() { }

	virtual void perform(uint8_t* imagergb) = 0;
};

