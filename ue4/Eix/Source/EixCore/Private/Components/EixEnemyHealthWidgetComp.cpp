#include "Components/EixEnemyHealthWidgetComp.h"
#include "UI/EixWidget_EnemyHealth.h"
#include "Log.h"

UEixEnemyHealthWidgetComp::UEixEnemyHealthWidgetComp()
{
	SetWidgetSpace(EWidgetSpace::Screen);
}

void UEixEnemyHealthWidgetComp::BeginPlay()
{
	Super::BeginPlay();
	UUserWidget* WidgetObject = GetUserWidgetObject();
	if (IsValid(WidgetObject))
	{
		checkf(WidgetObject->IsA<UEixWidget_EnemyHealth>(), TEXT("UEixEnemyHealthWidgetComp can only work with UEixWidget_EnemyHealth subclass as its widget"));
		EnemyHealthWidget = StaticCast<UEixWidget_EnemyHealth*>(WidgetObject);
	}
	else
	{
		UE_LOG(LogEixCore, Error, TEXT("No widget was provided to UEixEnemyHealthWidgetComp"));
	}
}

void UEixEnemyHealthWidgetComp::SetHealthPercentage(float In_HealthPercentage)
{
	if (EnemyHealthWidget.IsValid())
	{
		EnemyHealthWidget->SetTargetHealthPercentage(In_HealthPercentage);
	}
}
