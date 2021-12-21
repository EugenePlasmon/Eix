#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleInterface.h"

class FEixCore : public IModuleInterface
{
public:
    static inline FEixCore& Get()
    {
        return FModuleManager::LoadModuleChecked<FEixCore>("FEixCore");
    }

    static inline bool IsAvailable()
    {
        return FModuleManager::Get().IsModuleLoaded("EixCore");
    }

    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
};