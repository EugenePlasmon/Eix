#pragma once

#include "CoreMinimal.h"
#include "EixCharacterMovComp.h"
#include "EixPlayerCharacterMovComp.generated.h"

class AEixPlayerCharacter;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class EIXCORE_API UEixPlayerCharacterMovComp : public UEixCharacterMovComp
{
	GENERATED_BODY()

public:
	UEixPlayerCharacterMovComp();

	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	AEixPlayerCharacter* GetEixPlayerCharacterOwner() const { return EixPlayerCharacterOwner.Get(); }

private:
	TWeakObjectPtr<AEixPlayerCharacter> EixPlayerCharacterOwner;
};