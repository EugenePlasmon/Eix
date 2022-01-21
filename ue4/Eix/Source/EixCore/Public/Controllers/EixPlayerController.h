#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "EixPlayerController.generated.h"

UCLASS()
class EIXCORE_API AEixPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void SetPawn(APawn* InPawn) override;

	virtual void SetupInputComponent() override;

private:
	void MoveForward(float Value);
	void MoveRight(float Value);
	void Turn(float Value);
	void LookUp(float Value);

	void StartSprint();
	void StopSprint();

	void StartWalk();
	void StopWalk();

	void Roll();
	
	void PrimaryAttack_Pressed();
	void SecondaryAttack_Pressed();
	void SecondaryAttack_Released();
	
	TWeakObjectPtr<class AEixPlayerCharacter> EixPlayerCharacter;
};
