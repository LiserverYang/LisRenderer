# Copyright 2025, LiserverYang. All rights reserved.

from Build import BuildSystem

class RendererModule(BuildSystem.ModuleBase):
    """
    """

    def Configuration(self) -> None:
        """
        """

        self.BinaryType = BuildSystem.BinaryTypeEnum.DynamicLib
        self.ModulesDependOn = ["Eigen"]
        self.EnableBinaryLibPrefix = False