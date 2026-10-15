using UnrealBuildTool;
public class DAWSLegendTarget : TargetRules
{
    public DAWSLegendTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Game;
        DefaultBuildSettings = BuildSettingsVersion.V4;
        IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_4;
        ExtraModuleNames.Add("DAWSLegend");
    }
}