using System;
using System.IO;
using System.Linq;
using System.Reflection;
using Sharpmake;

namespace Extern
{
    [Generate]
    public class stb_image : Common.Project
    {
        public stb_image()
        {
        }

        public override void ConfigureAll(Configuration conf, Target target)
        {
            conf.IncludePaths.Add(".");
            conf.Output = Configuration.OutputType.None;
        }
    }
}