// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine.h"
#include "GameFramework/Character.h"
#include "Engine/TextureRenderTarget2D.h"
#include "SideScrollerCPPCharacter.generated.h"

UCLASS(config=Game)
class ASideScrollerCPPCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Side view camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* SideViewCameraComponent;

	/** Camera boom positioning the camera beside the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Access to MiniMap camera */
	class UCameraComponent* MiniMapCameraComponent;

	/** Access to MiniMap pixels */
	UTextureRenderTarget2D* MiniMapTextureRenderTarget;
	UTexture2D *MiniMapTexture;
	FRenderTarget *MiniMapRenderTarget;
	TArray<FColor> MiniMapSurfData;

	/** On-screen debugging */
	const FColor DEBUG_TEXT_COLOR = FColor::Yellow;
	const float  DEBUG_TEXT_SCALE = 2.f;

	void dbgprintf(const char* fmt, ...);

protected:

	/** Called for side to side input */
	void MoveRight(float Val);

	/** Handle touch inputs. */
	void TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location);

	/** Handle touch stop event. */
	void TouchStopped(const ETouchIndex::Type FingerIndex, const FVector Location);

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	// End of APawn interface


public:
	ASideScrollerCPPCharacter();

	/** Returns SideViewCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetSideViewCameraComponent() const { return SideViewCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	virtual void Tick(float DeltaSeconds) override;

};
