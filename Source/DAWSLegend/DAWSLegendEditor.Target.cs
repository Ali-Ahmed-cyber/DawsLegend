using UnrealBuildTool;
public class DAWSLegendEditorTarget : TargetRules
{
    public DAWSLegendEditorTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Editor;
        DefaultBuildSettings = BuildSettingsVersion.V4;
        IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_4;
        ExtraModuleNames.Add("DAWSLegend");
    }
}