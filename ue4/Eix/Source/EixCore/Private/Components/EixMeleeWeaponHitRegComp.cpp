#include "Components/EixMeleeWeaponHitRegComp.h"
#include "Actors/Weapons/EixMeleeWeapon.h"
#include "Components/CapsuleComponent.h"
#include "Subsystems/DebugSubsystem.h"
#include "Utils/EixTraceUtils.h"
#include "Types/EixCollisionTypes.h"

UEixMeleeWeaponHitRegComp::UEixMeleeWeaponHitRegComp()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UEixMeleeWeaponHitRegComp::BeginPlay()
{
	Super::BeginPlay();
	check(GetOwner()->IsA<AEixMeleeWeapon>());
	MeleeWeaponOwner = StaticCast<AEixMeleeWeapon*>(GetOwner());
}

void UEixMeleeWeaponHitRegComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (bHitRegEnabled)
	{
		const TArray<FSphericalColliderInfo> Colliders =
			GetSphericalCollidersForCapsuleComponent(MeleeWeaponOwner->GetHitRegistrationArea());
		if (CollidersPreviousInfo.Num() != 0)
		{
			ProcessHitRegistration(CollidersPreviousInfo, Colliders);
		}
		CollidersPreviousInfo = Colliders;
	}
}

void UEixMeleeWeaponHitRegComp::SetIgnoredActors(const FIgnoredActors& In_IgnoredActors)
{
	CollisionQueryParams.ClearIgnoredActors();
	CollisionQueryParams.AddIgnoredActors(In_IgnoredActors);
}

void UEixMeleeWeaponHitRegComp::SetHitRegEnabled(bool bEnabled)
{
	bHitRegEnabled = bEnabled;

	if (bEnabled)
	{
		CollidersPreviousInfo.Empty();
	}
}

TArray<FSphericalColliderInfo> UEixMeleeWeaponHitRegComp::GetSphericalCollidersForCapsuleComponent(
	const UCapsuleComponent* CapsuleComponent
) const
{
	const float Radius = CapsuleComponent->GetScaledCapsuleRadius();
	const float HalfHeight = CapsuleComponent->GetScaledCapsuleHalfHeight();
	const float Height = 2.f * HalfHeight;
	const FVector CapsuleLoc = CapsuleComponent->GetComponentLocation();
	const FRotator CapsuleRot = CapsuleComponent->GetComponentRotation();
	const FVector CapsuleUpVector = CapsuleRot.RotateVector(FVector::UpVector);
	
	const float NumberOfColliders = FMath::CeilToInt(Height / (MaxCollidersOffsetFraction * Radius));
	const float CollidersOffset = (Height - 2.f * Radius) / (NumberOfColliders - 1);
	
	TArray<FSphericalColliderInfo> Colliders;
	Colliders.Reserve(NumberOfColliders);
	for (int32 n = 0; n < NumberOfColliders; ++n)
	{
		const FVector Location = CapsuleLoc
			+ (HalfHeight - Radius - n * CollidersOffset) * CapsuleUpVector;
		Colliders.Emplace(Radius, Location);
	}
	return Colliders;
}

void UEixMeleeWeaponHitRegComp::ProcessHitRegistration(
	const TArray<FSphericalColliderInfo>& In_CollidersPreviousInfo,
	const TArray<FSphericalColliderInfo>& In_CollidersCurrentInfo
)
{
	// In theory these numbers may differ if the capsule changes it's size.
	// For now such cases are not supported by implementation
	check(In_CollidersCurrentInfo.Num() == In_CollidersPreviousInfo.Num());
	
	const int32 NumberOfColliders = In_CollidersCurrentInfo.Num();
	TArray<FHitResult> Hits;
	
	for (int32 n = 0; n < NumberOfColliders; ++n)
	{
		const FVector PreviousLocation = In_CollidersPreviousInfo[n].Location;
		const FVector CurrentLocation = In_CollidersCurrentInfo[n].Location;
		const float Radius = In_CollidersCurrentInfo[n].Radius;
		const bool bDrawDebug = GetWorld()->GetGameInstance()->GetSubsystem<UDebugSubsystem>()
		                                  ->IsDebugCategoryEnabled(DebugCategory::MeleeHitRegistration);
		
		TArray<FHitResult> ColliderHits;
		const bool bGotHit = EixTraceUtils::SweepSphereMultiByChannel(GetWorld(), ColliderHits,
			PreviousLocation, CurrentLocation, Radius, ECC_Melee,
			EixTraceUtils::DebugDrawParams(bDrawDebug, FColor::Green, FColor::Red, 3.f),
			CollisionQueryParams
		);
		
		if (!bGotHit)
		{
			continue;
		}
		
		for (const FHitResult& ColliderHit : ColliderHits)
		{
			bool bUniqueHit = true;
			for (const FHitResult& RegisteredHit : Hits)
			{
				if (ColliderHit.Component == RegisteredHit.Component)
				{
					bUniqueHit = false;
					break;
				}
			}
			if (bUniqueHit)
			{
				Hits.Add(ColliderHit);
				if (bIgnoreActorAfterHit)
				{
					CollisionQueryParams.AddIgnoredActor(ColliderHit.GetActor());
				}
			}
		}
	}

	if (Hits.Num() != 0)
	{
		if (OnMeleeHitRegistered.IsBound())
		{
			OnMeleeHitRegistered.Broadcast(Hits);
		}
	}
}
