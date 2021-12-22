#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EixCharacter.generated.h"

class UEixCharacterMovComp;

UCLASS()
class EIXCORE_API AEixCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AEixCharacter(const FObjectInitializer& ObjectInitializer);

	UEixCharacterMovComp* GetEixCharacterMovement() const { return EixCharacterMovement; }

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UEixCharacterMovComp* EixCharacterMovement;
};