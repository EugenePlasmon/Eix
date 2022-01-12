#pragma once

#include "CoreMinimal.h"
#include "EixCharacterMovComp.h"
#include "Components/TimelineComponent.h"
#include "EixPlayerCharacterMovComp.generated.h"

class AEixPlayerCharacter;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class EIXCORE_API UEixPlayerCharacterMovComp : public UEixCharacterMovComp
{
	GENERATED_BODY()

public:
	UEixPlayerCharacterMovComp();

#pragma region Overriden
protected:
	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	virtual void PhysCustom(float DeltaTime, int32 Iterations) override;

	virtual void PhysicsRotation(float DeltaTime) override;
#pragma endregion

#pragma region Owner
public:
	AEixPlayerCharacter* GetEixPlayerCharacterOwner() const { return EixPlayerCharacterOwner.Get(); }

private:
	TWeakObjectPtr<AEixPlayerCharacter> EixPlayerCharacterOwner;
#pragma endregion

#pragma region Rolling
public:
	bool IsRolling() const;
	bool CanStartRolling() const;
	void StartRolling();
	void StopRolling();

protected:
	/* Timeline's X axis is scaled to range [0, 1] */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Eix|Rolling")
	UCurveFloat* RollingSpeedCurve;

	/* Timeline's X axis is scaled to range [0, 1] */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Eix|Rolling")
	UCurveFloat* RollingRotationRateYawCurve;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Eix|Rolling")
	float RollingMaxDuration;
	
private:
	float InitialRollingSpeedXY = 0.f;
	float CurrentRollingSpeedZ = 0.f;
	float CurrentRollingAccumulatedTime = 0.f;

	FTimerHandle RollingTimer;
	
	void PhysRolling(float DeltaTime, int32 Iterations);
	void PhysicsRotationRolling(float DeltaTime);
#pragma endregion

#pragma region Misc
private:
	bool SweepCharacterCapsuleSingleByChannel(OUT FHitResult& OutHit, const FVector& EndStartDelta,
	                                          ECollisionChannel Channel) const;
#pragma endregion
};
