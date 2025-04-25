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

        self.ArgumentsAdded += ["-DCOMPILING",
                                f'-DCOMPILE_INFO=\"{BuildSystem.GetCurrentSystem().name}_x86_64, g++, Built by LisBuilder, {time.asctime(time.localtime(time.time()))}\"',
                                f"-DGITHUB_VERSION=\"{GithubVersion}\""]