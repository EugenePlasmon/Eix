#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "EixGameInstance.generated.h"

UCLASS()
class EIXCORE_API UEixGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	virtual bool ProcessConsoleExec(const TCHAR* Cmd, FOutputDevice& Ar, UObject* Executor) override;
};
