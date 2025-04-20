# Copyright 2025, LiserverYang. All rights reserved.

from Build import BuildSystem

class EigenModule(BuildSystem.ModuleBase):
    """
    """

    def Configuration(self) -> None:
        """
        """

        self.BinaryType = BuildSystem.BinaryTypeEnum.DynamicLib
        self.ModulesDependOn = []
        self.bBuildThisModule = True
        self.AutoSkiped = True
        self.LinkThisModule = False