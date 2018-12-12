#pragma once

/*
* ThreadedWorker.h: Header for running compute-intensive computations on their own thread
*
* Adapted from https://wiki.unrealengine.com/Multi-Threading:_How_to_Create_Threads_in_UE4
*
* Copyright (C) 2018 Simon D. Levy
*
* MIT License
*/

#include "Runtime/Core/Public/HAL/Runnable.h"
#include "GameFramework/Actor.h"

class FThreadedWorker : public FRunnable
{
	/** Singleton instance, can access the thread any time via static accessor, if it is active! */
	static  FThreadedWorker* Runnable;

	/** Thread to run the worker FRunnable on */
	FRunnableThread* Thread;

	/** Stop this thread? Uses Thread Safe Counter */
	FThreadSafeCounter StopTaskCounter;

public:

	//~~~ Thread Core Functions ~~~

	//Constructor / Destructor
	FThreadedWorker(void);

	virtual ~FThreadedWorker();

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
		This code ensures only one thread will be able to run at a time.
		This function returns a handle to the newly started instance.
	*/
	static FThreadedWorker* NewWorker(void);

	/** Shuts down the thread. Static so it can easily be called from outside the thread context */
	static void Shutdown();
};
