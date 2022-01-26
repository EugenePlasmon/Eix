#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EixMeleeWeapon.generated.h"

UCLASS()
class EIXCORE_API AEixMeleeWeapon : public AActor
{
	GENERATED_BODY()
	
public:
	AEixMeleeWeapon();

#pragma region Overriden
protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;
	
	virtual void OnConstruction(const FTransform& Transform) override;
#pragma endregion

#pragma region Components
public:
	class UCapsuleComponent* GetHitRegistrationArea() const { return HitRegistrationArea; }

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	class UCapsuleComponent* HitRegistrationArea;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	class UEixMeleeWeaponHitRegComp* HitRegComp;
#pragma endregion

#pragma region HitRegistration
public:
#if WITH_EDITORONLY_DATA
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Eix")
	bool bDrawCollidersInEditor = false;
#endif
	
	void SetHitRegistrationEnabled(bool bEnabled);
#pragma endregion
};
