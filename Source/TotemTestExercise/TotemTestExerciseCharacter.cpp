// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "TotemTestExerciseCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Materials/Material.h"
#include "Materials/MaterialInstance.h"
#include "Engine/World.h"
#include "Boat.h"

ATotemTestExerciseCharacter::ATotemTestExerciseCharacter()
{

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Create a decal in the world to show the cursor's location
	CursorToWorld = CreateDefaultSubobject<UDecalComponent>("CursorToWorld");
	CursorToWorld->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UMaterial> DecalMaterialAsset(TEXT("Material'/Game/TopDownCPP/Blueprints/M_Cursor_Decal.M_Cursor_Decal'"));
	if (DecalMaterialAsset.Succeeded())
	{
		CursorToWorld->SetDecalMaterial(DecalMaterialAsset.Object);
	}
	CursorToWorld->DecalSize = FVector(8.0f, 16.0f, 16.0f);
	CursorToWorld->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f).Quaternion());

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

void ATotemTestExerciseCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

	if (CursorToWorld != nullptr)
	{
		if (UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled())
		{
			if (UWorld* World = GetWorld())
			{
				FHitResult HitResult;
				FCollisionQueryParams Params(NAME_None, FCollisionQueryParams::GetUnknownStatId());
				FVector StartLocation = TopDownCameraComponent->GetComponentLocation();
				FVector EndLocation = TopDownCameraComponent->GetComponentRotation().Vector() * 2000.0f;
				Params.AddIgnoredActor(this);
				World->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, Params);
				FQuat SurfaceRotation = HitResult.ImpactNormal.ToOrientationRotator().Quaternion();
				CursorToWorld->SetWorldLocationAndRotation(HitResult.Location, SurfaceRotation);
			}
		}
		else if (APlayerController* PC = Cast<APlayerController>(GetController()))
		{
			FHitResult TraceHitResult;
			PC->GetHitResultUnderCursor(ECC_Visibility, true, TraceHitResult);
			FVector CursorFV = TraceHitResult.ImpactNormal;
			FRotator CursorR = CursorFV.Rotation();
			CursorToWorld->SetWorldLocation(TraceHitResult.Location);
			CursorToWorld->SetWorldRotation(CursorR);

			if (TraceHitResult.GetComponent() && !hoveredBoat && TraceHitResult.GetComponent()->ComponentHasTag("Head"))
			{
				hoveredBoat = Cast<ABoat>(TraceHitResult.GetActor()); //called once, when we hovered a gun
				hoveredBoat->ChangeHeadAndGunMaterial(true);
			}

			if (hoveredBoat && TraceHitResult.GetComponent() && !TraceHitResult.GetComponent()->ComponentHasTag("Head"))
			{
				if (!ChoosedBoats.Contains(hoveredBoat))
				{
					hoveredBoat->ChangeHeadAndGunMaterial(false);
				}
				
				hoveredBoat = NULL;
			}
		}
	}
}

void ATotemTestExerciseCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);

	InputComponent->BindAction("Choose", IE_Pressed, this, &ATotemTestExerciseCharacter::Choose);
}

void ATotemTestExerciseCharacter::ChooseGun()
{
	if (hoveredBoat)
	{
		if (!ChoosedBoats.Contains(hoveredBoat))
		{
			ChoosedBoats.Add(hoveredBoat);
		}

		else
		{
			ChoosedBoats.RemoveSwap(hoveredBoat);
			hoveredBoat->ChangeHeadAndGunMaterial(false);
		}
		
	}
}

void ATotemTestExerciseCharacter::AimToChoosedPoint(FVector point)
{

	for (ABoat* boat : ChoosedBoats)
	{
		boat->AimToPoint(point);
	}
}

void ATotemTestExerciseCharacter::Choose()
{
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		FHitResult TraceHitResult;
		PC->GetHitResultUnderCursor(ECC_Visibility, true, TraceHitResult);

		if (TraceHitResult.GetComponent() && TraceHitResult.GetComponent()->ComponentHasTag("Head"))
		{
			ChooseGun();
		}

		else
		{
			AimToChoosedPoint(TraceHitResult.Location);
		}
	}
}