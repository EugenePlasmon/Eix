#pragma once

#include "CoreMinimal.h"

namespace EixShapeUtils
{
	struct CapsuleParams
	{
		FVector Location;
		FQuat RotationQuat;
		float Radius;
		float HalfHeight;
		
		CapsuleParams(FVector Location, FQuat RotationQuat, float Radius, float HalfHeight)
			: Location(Location), RotationQuat(RotationQuat), Radius(Radius), HalfHeight(HalfHeight)
		{}
	};
	
	CapsuleParams MakeCapsuleBySweepingASphere(FVector Start, FVector End, float Radius);
}