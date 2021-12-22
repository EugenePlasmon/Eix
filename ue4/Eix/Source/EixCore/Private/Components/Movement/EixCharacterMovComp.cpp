#include "Components/Movement/EixCharacterMovComp.h"
#include "Characters/EixCharacter.h"

#include "GameFramework/Character.h"

UEixCharacterMovComp::UEixCharacterMovComp()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UEixCharacterMovComp::BeginPlay()
{
	Super::BeginPlay();
	checkf(GetCharacterOwner()->IsA<AEixCharacter>(), TEXT("UEixCharacterMovComp owner must be AEixCharacter"));
	EixCharacterOwner = StaticCast<AEixCharacter*>(GetCharacterOwner());
}

void UEixCharacterMovComp::TickComponent(float DeltaTime, ELevelTick TickType,
                                         FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

