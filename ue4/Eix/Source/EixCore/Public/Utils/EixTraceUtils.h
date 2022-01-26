#pragma once

#include "CoreMinimal.h"

namespace EixTraceUtils
{
	struct DebugDrawParams
	{
		bool bDrawDebug;
		FColor TraceColor;
		FColor HitColor;
		float DrawTime;

		static DebugDrawParams NoDebugDraw;

		DebugDrawParams(bool bDrawDebug, FColor TraceColor, FColor HitColor, float DrawTime = 0.f)
			: bDrawDebug(bDrawDebug), TraceColor(TraceColor), HitColor(HitColor), DrawTime(DrawTime)
		{}
		
		DebugDrawParams(FColor TraceColor, FColor HitColor, float DrawTime = 0.f)
			: bDrawDebug(true), TraceColor(TraceColor), HitColor(HitColor), DrawTime(DrawTime)
		{}

	private:
		DebugDrawParams()
			: bDrawDebug(false), DrawTime(0.f)
		{}
	};
	
	bool SweepSphereSingleByChannel
	(
		const UWorld* World,
		OUT struct FHitResult& OutHit,
		const FVector& Start,
		const FVector& End,
		float Radius,
		ECollisionChannel TraceChannel,
		const DebugDrawParams& DebugDrawParams = DebugDrawParams::NoDebugDraw,
		const FCollisionQueryParams& QueryParams = FCollisionQueryParams::DefaultQueryParam,
		const FCollisionResponseParams& ResponseParam = FCollisionResponseParams::DefaultResponseParam
	);

	bool SweepSphereMultiByChannel
	(
		const UWorld* World,
		OUT TArray<struct FHitResult>& OutHits,
		const FVector& Start,
		const FVector& End,
		float Radius,
		ECollisionChannel TraceChannel,
		const DebugDrawParams& DebugDrawParams = DebugDrawParams::NoDebugDraw,
		const FCollisionQueryParams& QueryParams = FCollisionQueryParams::DefaultQueryParam,
		const FCollisionResponseParams& ResponseParam = FCollisionResponseParams::DefaultResponseParam
	);
}
