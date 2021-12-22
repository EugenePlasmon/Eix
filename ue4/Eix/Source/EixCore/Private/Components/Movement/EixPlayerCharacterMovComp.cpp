#include "Components/Movement/EixPlayerCharacterMovComp.h"
#include "Characters/EixPlayerCharacter.h"

#include "GameFramework/Character.h"

UEixPlayerCharacterMovComp::UEixPlayerCharacterMovComp()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UEixPlayerCharacterMovComp::BeginPlay()
{
	Super::BeginPlay();
	checkf(GetCharacterOwner()->IsA<AEixCharacter>(), TEXT("UEixCharacterMovComp owner must be AEixCharacter"));
	EixPlayerCharacterOwner = StaticCast<AEixCharacter*>(GetCharacterOwner());
}

void UEixPlayerCharacterMovComp::TickComponent(float DeltaTime, ELevelTick TickType,
                                               FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
