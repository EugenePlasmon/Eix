#pragma once

#include "CoreMinimal.h"
#include "Characters/EixCharacter.h"
#include "EixAICharacter.generated.h"

class UEixCharacterHealthComp;
class UEixCharacterHitReactionComp;
class UEixEnemyHealthWidgetComp;

UCLASS()
class EIXCORE_API AEixAICharacter : public AEixCharacter
{
	GENERATED_BODY()

public:
	AEixAICharacter(const FObjectInitializer& ObjectInitializer);

#pragma region Overriden
public:
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
	                         AController* EventInstigator, AActor* DamageCauser) override;
	
protected:
	virtual void BeginPlay() override;
#pragma endregion 

#pragma region Components
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UEixCharacterHealthComp* HealthComp;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UEixCharacterHitReactionComp* HitReactionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Scene Components")
	UEixEnemyHealthWidgetComp* EnemyHealthWidgetComp;
#pragma endregion 
};
