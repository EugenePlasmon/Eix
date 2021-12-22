#pragma once

#include "CoreMinimal.h"
#include "Characters/EixCharacter.h"
#include "EixPlayerCharacter.generated.h"

class UEixPlayerCharacterMovComp;

UCLASS()
class EIXCORE_API AEixPlayerCharacter : public AEixCharacter
{
	GENERATED_BODY()

public:
	AEixPlayerCharacter(const FObjectInitializer& ObjectInitializer);

	virtual void Tick(float DeltaTime) override;

	virtual void BeginPlay() override;
	
	UEixPlayerCharacterMovComp* GetEixPlayerCharacterMovement() const { return EixPlayerCharacterMovement; }
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UEixPlayerCharacterMovComp* EixPlayerCharacterMovement;
};
