#include "Actors/Weapons/EixMeleeWeapon.h"
#include "Characters/EixCharacter.h"
#include "Components/EixMeleeWeaponHitRegComp.h"

#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"

AEixMeleeWeapon::AEixMeleeWeapon()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	
	HitRegistrationArea = CreateDefaultSubobject<UCapsuleComponent>(TEXT("HitRegistrationArea"));
	HitRegistrationArea->SetupAttachment(RootComponent);
	HitRegistrationArea->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	HitRegComp = CreateDefaultSubobject<UEixMeleeWeaponHitRegComp>(TEXT("HitRegComponent"));
}

void AEixMeleeWeapon::SetOwner(AActor* NewOwner)
{
	Super::SetOwner(NewOwner);
	if (IsValid(NewOwner))
	{
		check(NewOwner->IsA<AEixCharacter>());
		CharacterOwner = StaticCast<AEixCharacter*>(NewOwner);
	}
	else
	{
		CharacterOwner.Reset();
	}
}

void AEixMeleeWeapon::BeginPlay()
{
	Super::BeginPlay();
	HitRegComp->OnMeleeHitRegistered.AddDynamic(this, &AEixMeleeWeapon::OnMeleeHitRegistered);
}

void AEixMeleeWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEixMeleeWeapon::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

#if	WITH_EDITORONLY_DATA
	if (bDrawCollidersInEditor)
	{
		TArray<FSphericalColliderInfo> CollidersInfo =
			HitRegComp->GetSphericalCollidersForCapsuleComponent(GetHitRegistrationArea());
		
		for (const FSphericalColliderInfo& ColliderInfo: CollidersInfo)
		{
			USphereComponent* Collider = StaticCast<USphereComponent*>(
				AddComponentByClass(USphereComponent::StaticClass(), true,
				                    FTransform::Identity, false)
			);
			Collider->SetSphereRadius(ColliderInfo.Radius, false);
			Collider->SetWorldLocation(ColliderInfo.Location);
		}
	}
#endif
}

void AEixMeleeWeapon::SetHitRegistrationEnabled(bool bEnabled)
{
	if (bEnabled)
	{
		FIgnoredActors IgnoredActors;
		IgnoredActors.Add(this);
		if (IsValid(GetOwner()))
		{
			IgnoredActors.Add(GetOwner());
		}
		HitRegComp->SetIgnoredActors(IgnoredActors);
	}

	HitRegComp->SetHitRegEnabled(bEnabled);
}

void AEixMeleeWeapon::OnMeleeHitRegistered(TArray<FHitResult> Hits)
{
	for (const FHitResult& Hit: Hits)
	{
		if (!Hit.Actor.IsValid())
		{
			continue;
		}
		// TODO: Damage depends on current weapon and attack
		constexpr float DamageAmount = 10.f;
		const FVector Direction = (Hit.TraceEnd - Hit.TraceStart).GetSafeNormal();
		// TODO: Add damage type
		const TSubclassOf<UDamageType> DamageType = nullptr;
		FPointDamageEvent DamageEvent(DamageAmount, Hit, Direction, DamageType);
		AController* HitInstigator = CharacterOwner.IsValid() ? CharacterOwner->GetController() : nullptr;
		Hit.Actor->TakeDamage(DamageAmount, DamageEvent, HitInstigator, CharacterOwner.Get());
	}
}
