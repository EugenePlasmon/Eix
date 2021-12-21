using UnrealBuildTool;

public class EixTarget : TargetRules
{
    public EixTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Game;
        DefaultBuildSettings = BuildSettingsVersion.V2;
        ExtraModuleNames.AddRange( new string[] { "EixCore" } );
    }
}