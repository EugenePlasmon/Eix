#pragma once

#include "CoreMinimal.h"
#include "Characters/EixCharacter.h"
#include "Components/TimelineComponent.h"
#include "EixPlayerCharacter.generated.h"

class UEixPlayerCharacterMovComp;

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
#pragma endregion

#pragma region Components
public:
	UEixPlayerCharacterMovComp* GetEixPlayerCharacterMovement() const { return EixPlayerCharacterMovement; }
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UEixPlayerCharacterMovComp* EixPlayerCharacterMovement;
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

	UFUNCTION(BlueprintImplementableEvent)
	void OnStartRolling();
	
	FRotator GetAimOffset() const;

protected:
	virtual EEixGait GetCurrentAllowedGait() const override;

	bool CanSprint() const;
};
