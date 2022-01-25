#pragma once

#include "CoreMinimal.h"
#include "Characters/EixCharacter.h"
#include "EixAICharacter.generated.h"

class UEixCharacterHealthComp;

UCLASS()
class EIXCORE_API AEixAICharacter : public AEixCharacter
{
	GENERATED_BODY()

	AEixAICharacter(const FObjectInitializer& ObjectInitializer);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UEixCharacterHealthComp* HealthComp;
};
