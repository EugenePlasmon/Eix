#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EixCharacter.generated.h"

class UEixCharacterMovComp;
class UEixCharacterIKComp;

UCLASS()
class EIXCORE_API AEixCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AEixCharacter(const FObjectInitializer& ObjectInitializer);

	virtual void Tick(float DeltaSeconds) override;

	UEixCharacterMovComp* GetEixCharacterMovement() const { return EixCharacterMovement; }
	UEixCharacterIKComp* GetIKComp() const { return IKComp; }

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UEixCharacterMovComp* EixCharacterMovement;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UEixCharacterIKComp* IKComp;
};