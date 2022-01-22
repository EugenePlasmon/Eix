#pragma once

#include "CoreMinimal.h"
#include "Characters/EixCharacter.h"
#include "Components/TimelineComponent.h"
#include "EixPlayerCharacter.generated.h"

class UEixPlayerCharacterMovComp;
class UEixCharacterMeleeCombatComp;

UCLASS()
class EIXCORE_API AEixPlayerCharacter : public AEixCharacter
{
	GENERATED_BODY()

public:
	AEixPlayerCharacter(const FObjectInitializer& ObjectInitializer);

#pragma region Overriden
protected:
	virtual void Tick(float DeltaTime) override;

	virtual void BeginPlay() override;

	virtual void OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode) override;
#pragma endregion

#pragma region Components
public:
	UEixPlayerCharacterMovComp* GetEixPlayerCharacterMovement() const { return EixPlayerCharacterMovement; }
	UEixCharacterMeleeCombatComp* GetMeleeCombatComp() const { return MeleeCombatComp; };

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UEixPlayerCharacterMovComp* EixPlayerCharacterMovement;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UEixCharacterMeleeCombatComp* MeleeCombatComp;
#pragma endregion

public:
	virtual void MoveForward(float Value);
	virtual void MoveRight(float Value);
	virtual void Turn(float Value);
	virtual void LookUp(float Value);

	virtual void StartSprint();
	virtual void StopSprint();

	virtual void StartWalk();
	virtual void StopWalk();

	virtual void Roll();

	virtual void PrimaryAttack(EInputEvent InputEvent);
	virtual void SecondaryAttack(EInputEvent InputEvent);

	UFUNCTION(BlueprintImplementableEvent)
	void OnStartRolling();

	FRotator GetAimOffset() const;

protected:
	virtual EEixGait GetCurrentAllowedGait() const override;

	bool CanSprint() const;
};
