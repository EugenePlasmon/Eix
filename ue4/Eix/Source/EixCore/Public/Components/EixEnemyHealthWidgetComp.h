#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "EixEnemyHealthWidgetComp.generated.h"

class UEixWidget_EnemyHealth;

UCLASS()
class EIXCORE_API UEixEnemyHealthWidgetComp : public UWidgetComponent
{
	GENERATED_BODY()

public:
	UEixEnemyHealthWidgetComp();

#pragma region Overriden
protected:
	virtual void BeginPlay() override;
#pragma endregion

public:
	UFUNCTION(BlueprintCallable)
	void SetHealthPercentage(float In_HealthPercentage);

private:
	TWeakObjectPtr<UEixWidget_EnemyHealth> EnemyHealthWidget;
};
