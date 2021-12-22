#pragma once

#include "CoreMinimal.h"
#include "Characters/EixCharacter.h"
#include "EixPlayerCharacter.generated.h"

UCLASS()
class EIXCORE_API AEixPlayerCharacter : public AEixCharacter
{
	GENERATED_BODY()

public:
	AEixPlayerCharacter();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
};
