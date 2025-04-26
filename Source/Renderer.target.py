# Copyright 2025, LiserverYang. All rights reserved.

from Build import BuildSystem

import time
import subprocess

class RendererTarget(BuildSystem.TargetBase):
    """
    The target of renderer.
    """

    def Configuration(self) -> None:
        """
        Config renderer's config.
        """

        self.TargetType = BuildSystem.TargetTypeEnum.Program
        self.bBuildAllmodules = True
        self.ModulesSubFolder = [""]
        self.ArgumentsAdded = ["-std=c++20", "-Wno-deprecated-declarations", "-Wno-deprecated-enum-enum-conversion"]

        match BuildSystem.BuildContext.BuildType:
            case BuildSystem.BuildTypeEnum.Release:
                self.ArgumentsAdded += ["-O3", "-D__RELEASE__"]
            case BuildSystem.BuildTypeEnum.Debug:
                self.ArgumentsAdded += ["-O0", "-g", "-D__DEBUG__"]
            case BuildSystem.BuildTypeEnum.Development:
                # Developemt is also a kind of debug
                self.ArgumentsAdded += ["-O1", "-g", "-D__DEBUG__"]

        GithubVersion: str = subprocess.check_output(["git", "log", "--oneline", "-n", "1"]).split()[0].decode("utf-8")
        CompileInfo: str = f"{BuildSystem.GetCurrentSystem().name}_x86_64, g++ {BuildSystem.BuildContext.GxxVersionStr[0:-1]}, Built by LisBuilder {BuildSystem.LisBuilderVersionStr}, {time.asctime(time.localtime(time.time()))}"
        LicenseStr: str = ""

        with open("./LICENSE", 'r') as f:
            LicenseStr = f.read()

        Copyright: str = f"Copyright 2025, LiserverYang. All rights reserved.\n{LicenseStr}\""

        with open("./Source/Main/Public/Genericed.hpp", "w") as f:
            f.write(f'/**\n * Copyrigt 2025, LiserverYang. All rights reserved.\n * Genericed by BuildSystem.\n */\n\n#include "VersionInfo.hpp"\n\n\nconst std::string VERSION = lisRendererVersion + " " + "{GithubVersion}";\nconst std::string COMPILE_INFO = "{CompileInfo}";\nconst std::string COPYRIGHT = R"({Copyright})";')