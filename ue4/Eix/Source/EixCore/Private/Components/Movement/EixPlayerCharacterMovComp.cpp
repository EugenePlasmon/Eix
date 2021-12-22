#include "Components/Movement/EixPlayerCharacterMovComp.h"

UEixPlayerCharacterMovComp::UEixPlayerCharacterMovComp()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UEixPlayerCharacterMovComp::BeginPlay()
{
	Super::BeginPlay();
}

void UEixPlayerCharacterMovComp::TickComponent(float DeltaTime, ELevelTick TickType,
                                               FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
