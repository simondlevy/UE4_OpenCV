// Copyright (C) 2017 Simon D. Levy

#include "FlowVisionHUD.h"


// Sets default values
AFlowVisionHUD::AFlowVisionHUD()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AFlowVisionHUD::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFlowVisionHUD::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// RGB->gray formula from https ://www.johndcook.com/blog/2009/08/24/algorithms-convert-color-grayscale/
//imgdata[k] = (byte)(0.21 *PixelColor.R + 0.72 * PixelColor.G + 0.07 * PixelColor.B);