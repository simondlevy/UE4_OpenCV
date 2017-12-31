// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine.h"
#include "GameFramework/Character.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Runtime/Engine/Public/HighResScreenshot.h"

#include "VisionHUD.generated.h"

/**
 * 
 */
UCLASS()
class SIDESCROLLERCPP_API AVisionHUD : public AHUD
{
	GENERATED_BODY()

	/** Access to MiniMap camera stream */
	UCameraComponent* MiniMapCameraComponent;
	UTextureRenderTarget2D* MiniMapTextureRenderTarget;
	UTexture2D* MiniMapTexture;
	FRenderTarget* MiniMapRenderTarget;
	TArray<FColor> MiniMapSurfData;

	AVisionHUD();

	~AVisionHUD();
	
	virtual void DrawHUD() override;

	const float LEFTX = 1600.f;
	const float TOPY = 600.f;
	const float WIDTH = 256.f;
	const float HEIGHT = 128.f;

	const FLinearColor BORDER_COLOR = FLinearColor::Yellow;
	const float BORDER_WIDTH = 2.0f;

	void drawBorder(float lx, float uy, float rx, float by);

	void SaveRenderTargetToDisk(UTextureRenderTarget2D* InRenderTarget, FString Filename);


	bool wrotefile;
};
