#pragma once

#include "CoreMinimal.h"
#include "EixFootIKAnimParams.Generated.h"

USTRUCT(BlueprintType)
struct FEixFootIKAnimParams
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FVector PelvisOffset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FVector LeftFootEffectorLocation;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FVector RightFootEffectorLocation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FRotator LeftFootRotation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FRotator RightFootRotation;

	FEixFootIKAnimParams();
	FEixFootIKAnimParams(FVector PelvisOffset, FVector LeftFootEffectorLocation, FVector RightFootEffectorLocation,
	                     FRotator LeftFootRotation, FRotator RightFootRotation);
};

inline FEixFootIKAnimParams::FEixFootIKAnimParams()
	: PelvisOffset(0.f), LeftFootEffectorLocation(FVector::ZeroVector),
	  RightFootEffectorLocation(FVector::ZeroVector), LeftFootRotation(FRotator::ZeroRotator),
	  RightFootRotation(FRotator::ZeroRotator)
{}

inline FEixFootIKAnimParams::FEixFootIKAnimParams(FVector PelvisOffset, FVector LeftFootEffectorLocation,
                                                  FVector RightFootEffectorLocation,
                                                  FRotator LeftFootRotation, FRotator RightFootRotation)
	: PelvisOffset(PelvisOffset), LeftFootEffectorLocation(LeftFootEffectorLocation),
	  RightFootEffectorLocation(RightFootEffectorLocation), LeftFootRotation(LeftFootRotation),
	  RightFootRotation(RightFootRotation)
{}
