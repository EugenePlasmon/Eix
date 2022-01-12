#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Types/Character/EixGait.h"
#include "Types/Character/EixMovementSpecs.h"
#include "Types/Character/EixMovementState.h"
#include "EixCharacterMovComp.generated.h"

class AEixCharacter;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class EIXCORE_API UEixCharacterMovComp : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
	UEixCharacterMovComp();

#pragma region Overriden
protected:

	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	virtual void OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode) override;
#pragma endregion 

#pragma region Properties
public:
	AEixCharacter* GetEixCharacterOwner() const { return EixCharacterOwner.Get(); }
	
	EEixMovementState GetMovementState() const { return EixMovementState; }
	
	EEixGait GetCurrentGait() const { return CurrentGait; }

	/** Actual gait is determined by the current speed.
	 * For example, when a character is turning around, his move speed slows.
	 * Actual gait calculated like this is usually needed for more believable animation blends */
	EEixGait GetCurrentActualGait() const;

	float GetMoveSpeed() const { return MoveSpeed; }
	
	/** In local actor's coordinates */
	FVector GetVelocityAcceleration_LS() const { return VelocityAcceleration_LS; }
	
	void SetCurrentGait(EEixGait In_CurrentGait);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Eix|Specs")
	FDataTableRowHandle MovementSpecsTable;
	
	UPROPERTY(BlueprintReadOnly)
	FEixMovementSpecs MovementSpecs;

	UPROPERTY(BlueprintReadOnly)
	EEixMovementState EixMovementState = EEixMovementState::None;
	
private:
	EEixGait CurrentGait = EEixGait::Jog;

	FVector PrevVelocity = FVector::ZeroVector;
	FRotator PrevActorRotation = FRotator::ZeroRotator;

	FVector VelocityAcceleration = FVector::ZeroVector;
	FVector VelocityAcceleration_LS = FVector::ZeroVector;

	float MoveSpeed = 0.f;
	
	TWeakObjectPtr<AEixCharacter> EixCharacterOwner;
#pragma endregion 

private:
	void CacheCharacterOwner();

	void ReadMovementSpecs();

	void UpdateCharacterMovementValues(float DeltaTime);
};
