#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EixMeleeWeaponHitRegComp.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMeleeHitRegisteredSignature, TArray<FHitResult>, Hits);

typedef TArray<TWeakObjectPtr<const AActor>> FIgnoredActors;

struct FSphericalColliderInfo
{
	float Radius;
	FVector Location;
	
	FSphericalColliderInfo(float Radius, FVector Location): Radius(Radius), Location(Location) {}
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EIXCORE_API UEixMeleeWeaponHitRegComp : public UActorComponent
{
	GENERATED_BODY()

public:	
	UEixMeleeWeaponHitRegComp();

#pragma region Overriden
protected:
	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
#pragma endregion

public:
	FMeleeHitRegisteredSignature OnMeleeHitRegistered;
	
	void SetIgnoredActors(const FIgnoredActors& In_IgnoredActors);
	
	void SetHitRegEnabled(bool bEnabled);

	TArray<FSphericalColliderInfo> GetSphericalCollidersForCapsuleComponent(
		const class UCapsuleComponent* CapsuleComponent
	) const;

protected:
	/**
	 * Max distance between two spherical colliders, divided by collider's radius.
	 * 2 by default, which means spherical colliders will be strictly adjacent to each other
	 */
	float MaxCollidersOffsetFraction = 2.f;

	bool bIgnoreActorAfterHit = true;

private:
	bool bHitRegEnabled = false;
	TArray<FSphericalColliderInfo> CollidersPreviousInfo;
	FCollisionQueryParams CollisionQueryParams;

	TWeakObjectPtr<class AEixMeleeWeapon> MeleeWeaponOwner;

	void ProcessHitRegistration(const TArray<FSphericalColliderInfo>& In_CollidersPreviousInfo,
	                            const TArray<FSphericalColliderInfo>& In_CollidersCurrentInfo);
};
