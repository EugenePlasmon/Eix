#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Types/Character/EixGait.h"
#include "Types/Character/EixMovementSpecs.h"
#include "EixCharacter.generated.h"

class UEixCharacterMovComp;
class UEixCharacterIKComp;

UCLASS()
class EIXCORE_API AEixCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AEixCharacter(const FObjectInitializer& ObjectInitializer);

#pragma region Overriden
protected:
	virtual void Tick(float DeltaSeconds) override;

	virtual void BeginPlay() override;
#pragma endregion 

#pragma region Components
public:
	UEixCharacterMovComp* GetEixCharacterMovement() const { return EixCharacterMovement; }
	UEixCharacterIKComp* GetIKComp() const { return IKComp; }

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UEixCharacterMovComp* EixCharacterMovement;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UEixCharacterIKComp* IKComp;
#pragma endregion

#pragma region Properties
public:
	EEixGait GetDesiredGait() const { return DesiredGait; }
protected:
	virtual void SetDesiredGait(EEixGait In_DesiredGait);
private:
	EEixGait DesiredGait = EEixGait::Jog;

protected:
	virtual EEixGait GetCurrentAllowedGait() const;
#pragma endregion 
};