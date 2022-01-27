#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EixCharacterHealthComp.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EIXCORE_API UEixCharacterHealthComp : public UActorComponent
{
	GENERATED_BODY()

public:	
	UEixCharacterHealthComp();

#pragma region Overriden
protected:
	virtual void BeginPlay() override;
#pragma endregion 

public:

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Eix")
	float MaxHealth = 100.f;

private:
	float CurrentHealth;
};
