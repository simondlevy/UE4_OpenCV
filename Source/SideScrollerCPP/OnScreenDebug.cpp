/*
* OnScreenDebug: Simple on-screen debugging support for UnrealEngine4
*
* Copyright (C) 2017 Simon D. Levy
*
* MIT License
*/

#include "OnscreenDebug.h"

#include "Engine.h"

#include <stdarg.h>

// These params give a decent-sized, easily-readable display
static FColor TEXT_COLOR = FColor::Yellow;
static float  TEXT_SCALE = 2.f;

void Debug::printf(const char * fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	char buf[200];
	vsnprintf(buf, 200, fmt, ap);

	if (GEngine) {

		// 0 = overwrite; 5.0f = arbitrary time to display
		GEngine->AddOnScreenDebugMessage(0, 5.0f, TEXT_COLOR, FString(buf), true, FVector2D(TEXT_SCALE, TEXT_SCALE));
	}

	va_end(ap);

}