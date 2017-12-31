// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "SideScrollerCPPCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include <stdarg.h>
#include <stdio.h>

ASideScrollerCPPCharacter::ASideScrollerCPPCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Create a camera boom attached to the root (capsule)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bAbsoluteRotation = true; // Rotation of the character should not affect rotation of boom
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->TargetArmLength = 500.f;
	CameraBoom->SocketOffset = FVector(0.f,0.f,75.f);
	CameraBoom->RelativeRotation = FRotator(0.f,180.f,0.f);

	// Create a camera and attach to boom
	SideViewCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("SideViewCamera"));
	SideViewCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	SideViewCameraComponent->bUsePawnControlRotation = false; // We don't want the controller rotating the camera

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Face in the direction we are moving..
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->GravityScale = 2.f;
	GetCharacterMovement()->AirControl = 0.80f;
	GetCharacterMovement()->JumpZVelocity = 1000.f;
	GetCharacterMovement()->GroundFriction = 3.f;
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
	GetCharacterMovement()->MaxFlySpeed = 600.f;

	// Get Minimap render target from blueprint
	//static ConstructorHelpers::FObjectFinder<UTextureRenderTarget2D> MiniMapTexObj(TEXT("Texture2D'/Game/T_Minimap'"));
	static ConstructorHelpers::FObjectFinder<UTextureRenderTarget2D> MiniMapTexObj(TEXT("/Game/T_Minimap"));
	MiniMapTextureRenderTarget = MiniMapTexObj.Object;

	// Creates Texture2D to store MiniMapTex content
	MiniMapTexture = UTexture2D::CreateTransient(MiniMapTextureRenderTarget->SizeX, MiniMapTextureRenderTarget->SizeY, PF_B8G8R8A8);

#if WITH_EDITORONLY_DATA
	MiniMapTexture->MipGenSettings = TMGS_NoMipmaps;
#endif
	MiniMapTexture->SRGB = MiniMapTextureRenderTarget->SRGB;

	MiniMapRenderTarget = MiniMapTextureRenderTarget->GameThread_GetRenderTargetResource();

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

//////////////////////////////////////////////////////////////////////////
// Input

void ASideScrollerCPPCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASideScrollerCPPCharacter::MoveRight);

	PlayerInputComponent->BindTouch(IE_Pressed, this, &ASideScrollerCPPCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &ASideScrollerCPPCharacter::TouchStopped);
}

void ASideScrollerCPPCharacter::dbgprintf(const char * fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	char buf[200];
	vsnprintf(buf, 200, fmt, ap);

	if (GEngine) {

		// 0 = overwrite; 5.0f = arbitrary time to display
		GEngine->AddOnScreenDebugMessage(0, 5.0f, DEBUG_TEXT_COLOR, FString(buf), true, FVector2D(DEBUG_TEXT_SCALE, DEBUG_TEXT_SCALE));
	}

	va_end(ap);

}

void ASideScrollerCPPCharacter::Tick(float DeltaSeconds)
{

    // Adapted from https://answers.unrealengine.com/questions/193827/how-to-get-texture-pixels-using-utexturerendertarg.html

    // Read the pixels from the RenderTarget and store them in a FColor array
	MiniMapRenderTarget->ReadPixels(MiniMapSurfData);

    // Lock and copies the data between the textures
    /*
    void* TextureData = MiniMapTexture->PlatformData->Mips[0].BulkData.Lock(LOCK_READ_WRITE);
    const int32 TextureDataSize = SurfData.Num() * 4;
    FMemory::Memcpy(TextureData, SurfData.GetData(), TextureDataSize);
    MiniMapTexture->PlatformData->Mips[0].BulkData.Unlock();

    // Apply MiniMapTexture changes to GPU memory
    MiniMapTexture->UpdateResource();*/

    //FTexture2DMipMap* MyMipMap = &MiniMapTex->PlatformData->Mips[0];
    //int x = 0;
    //int y = 0;
    //dbgprintf("%d %d %d", PixelColor.R, PixelColor.G, PixelColor.B);

	/*
	for (int x = 0; x < MiniMapTextureRenderTarget->SizeX; ++x) {

		for (int y = 0; y < MiniMapTextureRenderTarget->SizeY; ++y) {

			FColor PixelColor = MiniMapSurfData[x + y * MiniMapTextureRenderTarget->SizeX];

			// RGB->gray formula from https ://www.johndcook.com/blog/2009/08/24/algorithms-convert-color-grayscale/
			float gray = 0.21 *PixelColor.R + 0.72 * PixelColor.G + 0.07 * PixelColor.B;
		}
	}

	dbgprintf("%3d FPS", (int)(1/DeltaSeconds));*/
}


void ASideScrollerCPPCharacter::MoveRight(float Value)
{
	// add movement in that direction
	AddMovementInput(FVector(0.f,-1.f,0.f), Value);
}

void ASideScrollerCPPCharacter::TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	// jump on any touch
	Jump();
}

void ASideScrollerCPPCharacter::TouchStopped(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	StopJumping();
}

