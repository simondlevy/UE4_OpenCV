#pragma once

/*
 * VisionAlgorithm.h: Abstract threaded machine-vision class for UnrealEngine4
 *
 * Adapted from https://wiki.unrealengine.com/Multi-Threading:_How_to_Create_Threads_in_UE4
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

    public:

        VisionAlgorithm(int width, int height);

        ~VisionAlgorithm();

        void update(cv::Mat & bgrimg);

        // Supports optional drawing to HUD
        virtual void draw(AHUD* hud, int leftx, int topy) { (void)hud; (void)leftx; (void)topy; }

        // FRunnable interface.
        virtual bool Init();
        virtual uint32 Run();
        virtual void Stop();

        // Makes sure this thread has stopped properly 
        void EnsureCompletion();

        /*
           Start the thread and the worker from static (easy access)!
           This code ensures only one thread will be able to run at a time.
           This function returns a handle to the newly started instance.
           */
        static VisionAlgorithm* NewWorker(int width, int height);

        // Shuts down the thread. Static so it can easily be called from outside the thread context
        static void Shutdown();

    protected:

        // Runs on its own thread
        virtual void perform(void) = 0;

		// Sent to update(); worked on by perform()
		cv::Mat * _bgrimg;

	private:

        // Singleton instance, can access the thread any time via static accessor, if it is active
        static  VisionAlgorithm* Runnable;

        // Thread to run the worker FRunnable on
        FRunnableThread* Thread;

        // Stop this thread? Uses Thread Safe Counter
        FThreadSafeCounter StopTaskCounter;
};

