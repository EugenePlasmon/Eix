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
	EEixMovementState GetMovementState() const { return MovementState; }
	EEixGait GetCurrentGait() const { return CurrentGait; }
	/* In local actor's coordinates */
	FVector GetVelocityAcceleration_LS() const { return VelocityAcceleration_LS; }
	
	void SetCurrentGait(EEixGait In_CurrentGait);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Eix|Specs")
	FDataTableRowHandle MovementSpecsTable;
	
	UPROPERTY(BlueprintReadOnly, Category="Eix|Specs")
	FEixMovementSpecs MovementSpecs;

private:
	EEixMovementState MovementState = EEixMovementState::None;
	EEixGait CurrentGait = EEixGait::Jog;

	FVector PrevVelocity = FVector::ZeroVector;
	FRotator PrevActorRotation = FRotator::ZeroRotator;

	FVector VelocityAcceleration = FVector::ZeroVector;
	FVector VelocityAcceleration_LS = FVector::ZeroVector;
	
	TWeakObjectPtr<AEixCharacter> EixCharacterOwner;

	void CacheCharacterOwner();

	void ReadMovementSpecs();
#pragma endregion 
};
