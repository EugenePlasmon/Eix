#include "UI/EixWidget_EnemyHealth.h"

void UEixWidget_EnemyHealth::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	HealthPercentage = FMath::FInterpTo(HealthPercentage, TargetHealthPercentage,
	                                    InDeltaTime, HealthPercentageInterpSpeed);
	ShadeHealthPercentage = FMath::FInterpTo(ShadeHealthPercentage, TargetHealthPercentage,
	                                         InDeltaTime, ShadeHealthPercentageInterpSpeed);
}

void UEixWidget_EnemyHealth::SetTargetHealthPercentage(float In_TargetHealthPercentage)
{
	const bool bHealthDecreased = In_TargetHealthPercentage < TargetHealthPercentage;
	TargetHealthPercentage = In_TargetHealthPercentage;
	if (bHealthDecreased)
	{
		HealthPercentage = TargetHealthPercentage;
	}
	else
	{
		ShadeHealthPercentage = TargetHealthPercentage;
	}
}
