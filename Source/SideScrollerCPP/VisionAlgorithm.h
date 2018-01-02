#pragma once

/*
* VisionAlgorithm.h: Abstract machine-vision class for UnrealEngine4
*
* Copyright (C) 2017 Simon D. Levy
*
* MIT License
*/

#include "Engine/TextureRenderTarget2D.h"
#include <cstdint>

class VisionAlgorithm {

public:

	virtual void perform(uint8_t* imagergb, UTextureRenderTarget2D* textureRenderTarget) = 0;

};

