#pragma once

/*
* EdgeDetection.h: OpenCV demo algorithm for UnrealEngine4
*
* Copyright (C) 2017 Simon D. Levy

* MIT License
*/

#include "Runtime/Core/Public/HAL/Runnable.h"
#include "GameFramework/Actor.h"

#include <opencv2/core.hpp>

class FEdgeDetection  : public FRunnable {

public:

	FEdgeDetection(int width, int height);

    ~FEdgeDetection();

	void update(cv::Mat & bgrimg);


	// Supports optional drawing to HUD
	/*virtual*/ void draw(AHUD* hud, int leftx, int topy); /* { (void)hud; (void)leftx; (void)topy; } */

	// FRunnable interface.
	virtual bool Init();
	virtual uint32 Run();
	virtual void Stop();

	/** Makes sure this thread has stopped properly */
	void EnsureCompletion();

	/*
	Start the thread and the worker from static (easy access)!
	This code ensures only one thread will be able to run at a time.
	This function returns a handle to the newly started instance.
	*/
	static FEdgeDetection* NewWorker(int width, int height);

	// Shuts down the thread. Static so it can easily be called from outside the thread context
	static void Shutdown();

private:

	cv::Mat * _bgrimg;

	// Arbitrary edge-detection params
	const int LOW_THRESHOLD = 50;
	const int RATIO         = 3;
	const int KERNEL_SIZE   = 3;

	FColor EDGE_COLOR = FColor::Green;

	// Runs on its own thread
	void perform(void);

	// Singleton instance, can access the thread any time via static accessor, if it is active
	static  FEdgeDetection* Runnable;

	// Thread to run the worker FRunnable on
	FRunnableThread* Thread;

	// Stop this thread? Uses Thread Safe Counter
	FThreadSafeCounter StopTaskCounter;

	// Stores vertices computed on thread
	static const int MAX_VERTICES = 1000;
	cv::Point _vertices[MAX_VERTICES];
	int _vertexCount;

	// Always gotta write this!
	static int min(int a, int b) { return a < b ? a : b; }
};
