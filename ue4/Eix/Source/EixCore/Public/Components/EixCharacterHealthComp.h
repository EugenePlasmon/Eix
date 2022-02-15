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
	float GetCurrentHealth() const { return CurrentHealth; }
	float GetCurrentHealthPercentage() const { return CurrentHealth / MaxHealth; }

	DECLARE_EVENT(UEixCharacterHealthComp, FHealthChangedEventSignature);
	FHealthChangedEventSignature& OnHealthChanged() { return HealthChangedEvent; }
	
	void TakeDamage(float DamageAmount);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Eix")
	float MaxHealth = 100.f;

private:
	float CurrentHealth;

	FHealthChangedEventSignature HealthChangedEvent;
};
