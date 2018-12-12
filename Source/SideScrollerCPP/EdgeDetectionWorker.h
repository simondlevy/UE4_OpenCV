#pragma once

/*
* EdgeDetectionWorker.h: Header for running OpenCV edge detection on its own thread
*
* Adapted from https://wiki.unrealengine.com/Multi-Threading:_How_to_Create_Threads_in_UE4
*
* Copyright (C) 2018 Simon D. Levy
*
* MIT License
*/

#include "Runtime/Core/Public/HAL/Runnable.h"
#include "GameFramework/Actor.h"

class FEdgeDetectionWorker : public FRunnable
{
	/** Singleton instance, can access the thread any time via static accessor, if it is active! */
	static  FEdgeDetectionWorker* Runnable;

	/** Thread to run the worker FRunnable on */
	FRunnableThread* Thread;

	/** The Data Ptr */
	TArray<uint32>* EdgeDetections;

	/** The PC */
	//AVictoryGamePlayerController* ThePC;

	/** Stop this thread? Uses Thread Safe Counter */
	FThreadSafeCounter StopTaskCounter;

private:
	int32				PrimesFoundCount;
public:

	int32				TotalPrimesToFind;

	//Done?
	bool IsFinished() const
	{
		return PrimesFoundCount >= TotalPrimesToFind;
	}

	//~~~ Thread Core Functions ~~~

	//Constructor / Destructor
	FEdgeDetectionWorker(TArray<uint32>& TheArray, const int32 IN_PrimesToFindPerTick);

	virtual ~FEdgeDetectionWorker();

	// Begin FRunnable interface.
	virtual bool Init();
	virtual uint32 Run();
	virtual void Stop();
	// End FRunnable interface

	/** Makes sure this thread has stopped properly */
	void EnsureCompletion();



	//~~~ Starting and Stopping Thread ~~~



	/*
		Start the thread and the worker from static (easy access)!
		This code ensures only 1 Prime Number thread will be able to run at a time.
		This function returns a handle to the newly started instance.
	*/
	static FEdgeDetectionWorker* JoyInit(TArray<uint32>& TheArray, const int32 IN_TotalPrimesToFind);

	/** Shuts down the thread. Static so it can easily be called from outside the thread context */
	static void Shutdown();

        static bool IsThreadFinished();

};
