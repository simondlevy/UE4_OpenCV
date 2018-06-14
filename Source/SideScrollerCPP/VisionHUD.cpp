/*
* VisionHUD: Heads-Up Display class for machine vision projects with UnrealEngine4
*
* Adapted from https://answers.unrealengine.com/questions/193827/how-to-get-texture-pixels-using-utexturerendertarg.html
*
* Copyright (C) 2017 Simon D. Levy
*
* MIT License
*/

#include "VisionHUD.h"

// Use whatever machine-vision algorithm you like
#include "EdgeDetection.h"

AVisionHUD::AVisionHUD()
{
	// Get Minimap render target from blueprint
	static ConstructorHelpers::FObjectFinder<UTextureRenderTarget2D> MiniMapTexObj(RENDER_TARGET_NAME);
	MiniMapTextureRenderTarget = MiniMapTexObj.Object;

	// Create Texture2D to store MiniMapTex content
	UTexture2D* MiniMapTexture = UTexture2D::CreateTransient(MiniMapTextureRenderTarget->SizeX, MiniMapTextureRenderTarget->SizeY, RENDER_TARGET_PIXEL_FORMAT);

#if WITH_EDITORONLY_DATA
	MiniMapTexture->MipGenSettings = TMGS_NoMipmaps;
#endif
	MiniMapTexture->SRGB = MiniMapTextureRenderTarget->SRGB;

	MiniMapRenderTarget = MiniMapTextureRenderTarget->GameThread_GetRenderTargetResource();

	// Allocate memory for RGB image bytes
	rows = MiniMapTextureRenderTarget->SizeY;
	cols = MiniMapTextureRenderTarget->SizeX;
	imagergb = new uint8_t[rows*cols*3];

	// Specify a machine-vision algorithm
	algorithm = new EdgeDetection(this, LEFTX, TOPY, rows, cols);
}

AVisionHUD::~AVisionHUD()
{
	delete imagergb;
	delete algorithm;
}

void AVisionHUD::DrawHUD()
{
	Super::DrawHUD();	

	// Draw the image to the HUD
	DrawTextureSimple(MiniMapTextureRenderTarget, LEFTX, TOPY, 1.0f, true);

	// Read the pixels from the RenderTarget and store them in a FColor array
	MiniMapRenderTarget->ReadPixels(MiniMapSurfData);

	// Convert the FColor array to an RGB byte array
	for (int x = 0; x < cols; ++x) {

		for (int y = 0; y < rows; ++y) {

			int k = x + y * cols;

			FColor PixelColor = MiniMapSurfData[k];

			imagergb[k*3]   = PixelColor.R;
			imagergb[k*3+1] = PixelColor.G;
			imagergb[k*3+2] = PixelColor.B;
		}
	}

	// Draw a border around the image

	float rightx = LEFTX + cols;
	float bottomy = TOPY + rows;

	drawBorder(LEFTX, TOPY, rightx, TOPY);
	drawBorder(rightx, TOPY, rightx, bottomy);
	drawBorder(rightx, bottomy, LEFTX, bottomy);
	drawBorder(LEFTX, bottomy, LEFTX, TOPY);

	// Run your vision algorithm
	algorithm->perform(imagergb);
}

void AVisionHUD::drawBorder(float lx, float uy, float rx, float by)
{
	DrawLine(lx, uy, rx, by, BORDER_COLOR, BORDER_WIDTH);
}
