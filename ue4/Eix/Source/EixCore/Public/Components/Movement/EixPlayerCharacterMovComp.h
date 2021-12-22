#pragma once

#include "CoreMinimal.h"
#include "EixCharacterMovComp.h"
#include "EixPlayerCharacterMovComp.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class EIXCORE_API UEixPlayerCharacterMovComp : public UEixCharacterMovComp
{
	GENERATED_BODY()

public:
	UEixPlayerCharacterMovComp();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};