using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Reflection;
using Sharpmake;

[module: Sharpmake.Include("*.cs")]

public static class Common
{
    public static ITarget[] GetDefaultTargets()
    {
        return new ITarget[]
        {
            new Target(
                Platform.win64 | Platform.win32,
                DevEnv.vs2017,
                Optimization.Debug | Optimization.Release
            )
        };
    }

    public abstract class Project : Sharpmake.Project
    {
        public Project()
        {
            AddTargets(GetDefaultTargets());

            string ns = GetType().Namespace;
            ns = string.IsNullOrEmpty(ns) ? "" : (ns.Replace('.', '/') + '/');

            RootPath = @"[project.SharpmakeCsPath]/../projects/" + ns;
            SourceRootPath = "../../" + ns + "[project.Name]/";
        }

        [Configure]
        public abstract void ConfigureAll(Configuration conf, Target target);

        [Configure]
        [ConfigurePriority(-1)]
        public virtual void PreConfigure(Configuration conf, Target target)
        {
            conf.ProjectPath = RootPath;
            conf.ProjectFileName = "[project.Name]";
            conf.TargetPath = @"[project.RootPath]/lib/[project.Name]/[target.Optimization]";
            conf.IntermediatePath = @"[project.RootPath]/obj/[project.Name]/[target.Optimization]";
            conf.Output = Configuration.OutputType.Lib;
            conf.Options.Add(Sharpmake.Options.Vc.Compiler.CppLanguageStandard.CPP17);
            conf.Options.Add(Sharpmake.Options.Vc.Compiler.Exceptions.Enable);
            conf.Options.Add(Sharpmake.Options.Vc.General.WindowsTargetPlatformVersion.v10_0_17763_0);
        }
    }
}

[Generate]
public class LatencySampleSolution : Solution
{
    public LatencySampleSolution()
    {
        Name = "LatencySample";

        AddTargets(Common.GetDefaultTargets());
    }

    [Configure]
    public virtual void ConfigureAll(Configuration conf, Target target)
    {
        conf.SolutionFileName = @"[solution.Name]";
        conf.SolutionPath = @"[solution.SharpmakeCsPath]/../projects/";

        conf.AddProject<LatencySample>(target);
    }
}

[Generate]
public class LatencySampleRootSolution : LatencySampleSolution
{
    public LatencySampleRootSolution()
    {
    }
    
    public override void ConfigureAll(Configuration conf, Target target)
    {
        base.ConfigureAll(conf, target);
        conf.SolutionPath = @"[solution.SharpmakeCsPath]/../../";
    }
}

public static class Main
{
    [Sharpmake.Main]
    public static void SharpmakeMain(Arguments arguments)
    {
        arguments.Generate<LatencySampleSolution>();
        arguments.Generate<LatencySampleRootSolution>();
    }
}
