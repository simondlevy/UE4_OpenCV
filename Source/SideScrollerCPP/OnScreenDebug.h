/*
* OnScreenDebug: Simple on-screen debugging support for UnrealEngine4
*
* Copyright (C) 2017 Simon D. Levy
*
* MIT License
*/

#pragma once

#include "CoreMinimal.h"

/**
 *  Provides a C-like printf() method for onscreen debugging.
 */
class Debug
{
public:

	static void printf(const char * fmt, ...);

};
