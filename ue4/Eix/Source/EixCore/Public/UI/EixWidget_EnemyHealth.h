#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EixWidget_EnemyHealth.generated.h"

UCLASS()
class EIXCORE_API UEixWidget_EnemyHealth : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	UFUNCTION(BlueprintCallable)
	void SetTargetHealthPercentage(float In_TargetHealthPercentage);
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Config")
	float HealthPercentageInterpSpeed = 5.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Config")
	float ShadeHealthPercentageInterpSpeed = 2.5f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Bindings")
	float HealthPercentage = 1.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Bindings")
	float ShadeHealthPercentage = 1.f;

private:
	float TargetHealthPercentage = 1.f;
};
