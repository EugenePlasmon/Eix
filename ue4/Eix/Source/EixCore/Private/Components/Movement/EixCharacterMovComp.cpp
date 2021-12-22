#include "Components/Movement/EixCharacterMovComp.h"

UEixCharacterMovComp::UEixCharacterMovComp()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UEixCharacterMovComp::BeginPlay()
{
	Super::BeginPlay();
}

void UEixCharacterMovComp::TickComponent(float DeltaTime, ELevelTick TickType,
                                         FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

