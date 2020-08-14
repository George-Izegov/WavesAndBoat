// Fill out your copyright notice in the Description page of Project Settings.


#include "Boat.h"
#include "Water.h"
#include "Engine/World.h"

// Sets default values
ABoat::ABoat()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABoat::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABoat::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABoat::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

FRotator ABoat::GetRotationByWaveHeights(TArray<float> heights)
{
	float z1z4AvgHeight = (heights[0] + heights[3]) / 2;
	float z2z3AvgHeight = (heights[1] + heights[2]) / 2;
	float z1z2AvgHeight = (heights[0] + heights[1]) / 2;
	float z3z4AvgHeight = (heights[2] + heights[3]) / 2;

	float deltaYHeight = z1z4AvgHeight - z2z3AvgHeight;
	float deltaXHeight = z1z2AvgHeight - z3z4AvgHeight;

	float xRotation = atan(deltaYHeight / BoatWidth)*(-1) * 180 / PI;
	float yRotation = atan(deltaXHeight / BoatLength) * 180 / PI;

	FRotator res;
	res.Roll = xRotation;
	res.Pitch = yRotation/2;
	res.Yaw = 0.0;
	return res;
}

TArray<float> ABoat::GetWaveHeights(TArray<FVector> BoatPoints)
{
	TArray<float> heights;

	for (auto point : BoatPoints)
	{
		heights.Add(Water->GetWaveHeightAtPosition(point));
	}

	return heights;
}

void ABoat::CalculateBoatWidthLength(FVector x1, FVector x2, FVector x3, FVector x4)
{
	BoatLength = x1.Distance(x1, x4);
	BoatWidth = x1.Distance(x1, x2);
}

FRotator ABoat::GetNewBoatRotation(TArray<FVector> BoatPoints)
{
	TArray<float> heights = GetWaveHeights(BoatPoints);

	return GetRotationByWaveHeights(heights);
}

void ABoat::SetNewBoatHeight()
{
	FVector BoatLocation = GetActorLocation();
	float waveHeight = Water->GetWaveHeightAtPosition(BoatLocation);
	BoatLocation.Z = Water->GetActorLocation().Z + waveHeight;

	SetActorLocation(BoatLocation);

}

