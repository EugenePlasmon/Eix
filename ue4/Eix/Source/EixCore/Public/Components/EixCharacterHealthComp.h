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

};
