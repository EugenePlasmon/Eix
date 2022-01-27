#include "Utils/EixTraceUtils.h"
#include "Utils/EixShapeUtils.h"
#include "DrawDebugHelpers.h"

EixTraceUtils::DebugDrawParams EixTraceUtils::DebugDrawParams::NoDebugDraw;

bool EixTraceUtils::SweepSphereSingleByChannel
(
	const UWorld* World,
	OUT struct FHitResult& OutHit,
	const FVector& Start,
	const FVector& End,
	float Radius,
	ECollisionChannel TraceChannel,
	const DebugDrawParams& DebugDrawParams,
	const FCollisionQueryParams& QueryParams,
	const FCollisionResponseParams& ResponseParam
)
{
	const FCollisionShape Sphere = FCollisionShape::MakeSphere(Radius);
	const bool bResult = World->SweepSingleByChannel(OutHit, Start, End, FQuat::Identity,
	                                                 TraceChannel, Sphere,
	                                                 QueryParams, ResponseParam);

	if (DebugDrawParams.bDrawDebug)
	{
		const EixShapeUtils::CapsuleParams Capsule =
			EixShapeUtils::MakeCapsuleSweepingSphere(Start, End, Radius);
		const FColor Color = bResult ? DebugDrawParams.HitColor : DebugDrawParams.TraceColor;
		DrawDebugCapsule(World, Capsule.Location, Capsule.HalfHeight, Capsule.Radius,
						 Capsule.RotationQuat, Color, false, DebugDrawParams.DrawTime);
		if (bResult)
		{
			constexpr float PointSize = 12.f;
			DrawDebugPoint(World, OutHit.ImpactPoint, PointSize,
			               DebugDrawParams.HitColor, false, DebugDrawParams.DrawTime);
		}
	}
	return bResult;
}

bool EixTraceUtils::SweepSphereMultiByChannel
(
	const UWorld* World,
	OUT TArray<struct FHitResult>& OutHits,
	const FVector& Start,
	const FVector& End,
	float Radius,
	ECollisionChannel TraceChannel,
	const DebugDrawParams& DebugDrawParams,
	const FCollisionQueryParams& QueryParams,
	const FCollisionResponseParams& ResponseParam
)
{
	const FCollisionShape Sphere = FCollisionShape::MakeSphere(Radius);
	const bool bResult = World->SweepMultiByChannel(OutHits, Start, End, FQuat::Identity,
	                                                TraceChannel, Sphere,
	                                                QueryParams, ResponseParam);

	if (DebugDrawParams.bDrawDebug)
	{
		const EixShapeUtils::CapsuleParams Capsule =
			EixShapeUtils::MakeCapsuleSweepingSphere(Start, End, Radius);
		const FColor Color = bResult ? DebugDrawParams.HitColor : DebugDrawParams.TraceColor;
		DrawDebugCapsule(World, Capsule.Location, Capsule.HalfHeight, Capsule.Radius,
		                 Capsule.RotationQuat, Color, false, DebugDrawParams.DrawTime);
		if (bResult)
		{
			for (const FHitResult& Hit : OutHits)
			{
				constexpr float PointSize = 12.f;
				DrawDebugPoint(World, Hit.ImpactPoint, PointSize,
				               DebugDrawParams.HitColor, false, DebugDrawParams.DrawTime);
			}
		}
	}
	return bResult;
}
