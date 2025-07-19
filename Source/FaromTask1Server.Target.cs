using UnrealBuildTool;
using System.Collections.Generic;

public class FaromTask1ServerTarget : TargetRules
{
    public FaromTask1ServerTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Server;
        DefaultBuildSettings = BuildSettingsVersion.V5;
        ExtraModuleNames.Add("FaromTask1");
      
    }
}