// Fill out your copyright notice in the Description page of Project Settings.


#include "Water.h"
#include "Engine/World.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Materials/MaterialParameterCollection.h"


// Sets default values
AWater::AWater()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SetWaveLength(4000);
	SetWaveHeight(10);
	SetWaveDirection({ 1,0,0 });
}

// Called when the game starts or when spawned
void AWater::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AWater::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

float AWater::GetWaveHeightAtPosition(FVector Position)
{
	float time = GetWorld()->TimeSeconds;
	float kx = WaveDirection.DotProduct(Position, 2 * PI / WaveLength * WaveDirection);
	float lambdat = sqrt(2 * PI / WaveLength * 980)*time;

	return WaveAmplitudeHeight * cos(kx - lambdat);
}

void AWater::SetWaveLength(float Length)
{
	WaveLength = Length;
	UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), MPC_Water, "Length", Length);
}

void AWater::SetWaveHeight(float Height)
{
	WaveAmplitudeHeight = Height;
	UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), MPC_Water, "Height", Height);
}

void AWater::SetWaveDirection(FVector Direction)
{
	WaveDirection = Direction;
	WaveDirection.Normalize();
	UKismetMaterialLibrary::SetVectorParameterValue(GetWorld(), MPC_Water, "Direction", Direction);
}


