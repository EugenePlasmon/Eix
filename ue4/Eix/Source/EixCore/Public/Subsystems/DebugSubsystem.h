#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "DebugSubsystem.generated.h"

namespace DebugCategory
{
	extern const FName IKCharacter;
	extern const FName MeleeHitRegistration;

	extern const FName All;
}

UCLASS()
class EIXCORE_API UDebugSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	bool IsDebugCategoryEnabled(const FName& Category) const;
	
private:
	UFUNCTION(Exec)
	void DebugCategorySetEnabled(const FName& Category_In, bool bEnabled_In);

	TMap<FName, bool> DebugCategoryEnabledInfo;
};
