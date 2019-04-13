using System;
using System.IO;
using System.Linq;
using System.Reflection;
using Sharpmake;

namespace Extern
{
    [Generate]
    public class FreeType : Common.Project
    {
        public FreeType()
        {
        }

        public override void ConfigureAll(Configuration conf, Target target)
        {
            conf.IncludePaths.Add("include");
            conf.IncludePrivatePaths.Add("internal_include");
            conf.SourceFilesFiltersRegex.Add(@"src\\([^\\]*)\\(ft)?\1\.*");
            conf.Defines.Add("FT2_BUILD_LIBRARY");
        }
    }
}