#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EixCharacterMovComp.generated.h"

class AEixCharacter;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class EIXCORE_API UEixCharacterMovComp : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
	UEixCharacterMovComp();

	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	AEixCharacter* GetEixCharacterOwner() const { return EixCharacterOwner.Get(); }

private:
	TWeakObjectPtr<AEixCharacter> EixCharacterOwner;
};
