#pragma once

/*
* VisionAlgorithm.h: Abstract threaded machine-vision class for UnrealEngine4
*
* Copyright (C) 2017 Simon D. Levy
*
* MIT License
*/

#include "Engine.h"

#include "Runtime/Core/Public/HAL/Runnable.h"
#include "GameFramework/Actor.h"

#include <opencv2/core.hpp>

/**
*  Abstract threaded machine-vision class.
*/
class VisionAlgorithm  : public FRunnable {

};

