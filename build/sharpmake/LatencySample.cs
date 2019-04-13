using System;
using System.IO;
using System.Linq;
using System.Reflection;
using Sharpmake;

[Generate]
public class LatencySample : Common.Project
{
    public LatencySample()
    {
    }
    
    public override void ConfigureAll(Configuration conf, Target target)
    {
        conf.AddPublicDependency<Extern.SFML>(target);

        conf.Output = Configuration.OutputType.Exe;
        conf.Options.Add(Sharpmake.Options.Vc.Linker.SubSystem.Application);
        conf.Options.Add(Sharpmake.Options.Vc.Linker.GenerateMapFile.Disable);

        conf.IncludePaths.Add("public");

        conf.TargetPath = "[project.SharpmakeCsPath]/../../bin/[target.Optimization]/";
    }
}
