#include "Utils/EixShapeUtils.h"

namespace EixShapeUtils
{
	CapsuleParams MakeCapsuleSweepingSphere(FVector Start, FVector End, float Radius)
	{
		const FVector Center = (Start + End) / 2.f;
		const FVector CapsuleUpVector = End - Start;
		const float HalfHeight = CapsuleUpVector.Size() / 2.f + Radius;
		const FQuat Quat = FRotationMatrix::MakeFromZ(CapsuleUpVector).ToQuat();
		return CapsuleParams(Center, Quat, Radius, HalfHeight);
	}
}
