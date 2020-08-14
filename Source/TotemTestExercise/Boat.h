// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Boat.generated.h"

UCLASS(Blueprintable)
class TOTEMTESTEXERCISE_API ABoat : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABoat();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class AWater* Water;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float BoatWidth;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float BoatLength;


protected:
	FRotator GetRotationByWaveHeights(TArray<float> heights);

	TArray<float> GetWaveHeights(TArray<FVector> BoatPoints);

	UFUNCTION(BlueprintCallable)
	void CalculateBoatWidthLength(FVector x1, FVector x2, FVector x3, FVector x4);

	UFUNCTION(BlueprintCallable)
	FRotator GetNewBoatRotation(TArray<FVector> BoatPoints);

	UFUNCTION(BlueprintCallable)
	void SetNewBoatHeight();

public:

	UFUNCTION(BlueprintImplementableEvent)
	void ChangeHeadAndGunMaterial(bool isHovered);

	UFUNCTION(BlueprintImplementableEvent)
	void AimToPoint(FVector point);


};
