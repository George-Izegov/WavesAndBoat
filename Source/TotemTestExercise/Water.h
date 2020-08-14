// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Water.generated.h"

UCLASS(Blueprintable)
class TOTEMTESTEXERCISE_API AWater : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWater();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION(BlueprintCallable)
		float GetWaveHeightAtPosition(FVector Position);

	UFUNCTION(BlueprintCallable)
		void SetWaveLength(float Length);

	UFUNCTION(BlueprintCallable)
		void SetWaveHeight(float Height);

	UFUNCTION(BlueprintCallable)
		void SetWaveDirection(FVector Direction);

	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float WaveAmplitudeHeight;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float WaveLength;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector WaveDirection;

protected:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UMaterialParameterCollection* MPC_Water;
};
