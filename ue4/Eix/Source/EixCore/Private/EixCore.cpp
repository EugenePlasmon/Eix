#include "EixCore.h"
#include "Log.h"

#include "Modules/ModuleManager.h"

void FEixCore::StartupModule()
{
    UE_LOG(LogEixCore, Log, TEXT("EixCore module starting up"));
}

void FEixCore::ShutdownModule()
{
    UE_LOG(LogEixCore, Log, TEXT("EixCore module shutting down"));
}

IMPLEMENT_PRIMARY_GAME_MODULE(FEixCore, EixCore, "EixCore");