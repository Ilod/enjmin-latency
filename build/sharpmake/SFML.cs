using System;
using System.IO;
using System.Linq;
using System.Reflection;
using Sharpmake;

namespace Extern
{
    [Generate]
    public class SFML : Common.Project
    {
        public SFML()
        {
        }

        public override void ConfigureAll(Configuration conf, Target target)
        {
            conf.SourceFilesFiltersRegex.Add(@"src\\SFML\\[^\\]*\\[^\\]*\.[^\\]*");
            conf.AddPrivateDependency<FreeType>(target);
            conf.AddPrivateDependency<stb_image>(target);
            conf.IncludePaths.Add("include");
            conf.IncludePrivatePaths.Add("src");
            conf.Defines.Add("SFML_STATIC");
            conf.ExportDefines.Add("SFML_STATIC");
        }

        [Configure(Platform.win32 | Platform.win64)]
        public void ConfigureWindows(Configuration conf, Target target)
        {
            conf.SourceFilesFiltersRegex.Add(@"src\\SFML\\[^\\]*\\Win32\\");
            conf.LibraryFiles.Add("opengl32", "winmm", "ws2_32");
        }
    }
}