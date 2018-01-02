// Copyright (C) 2017 Simon D. Levy

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "VisionHUD.h"

#include "FlowVisionHUD.generated.h"

UCLASS()
class SIDESCROLLERCPP_API AFlowVisionHUD : public AVisionHUD
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFlowVisionHUD();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
