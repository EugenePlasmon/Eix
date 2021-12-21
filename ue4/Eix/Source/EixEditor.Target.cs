using UnrealBuildTool;

public class EixEditorTarget : TargetRules
{
    public EixEditorTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Editor;
        DefaultBuildSettings = BuildSettingsVersion.V2;
        ExtraModuleNames.AddRange( new string[] { "EixCore" } );
    }
}