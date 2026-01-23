#!/usr/bin/env python
# -*- coding: utf-8 -*-
#Author : Greedysky

import os
import platform

class QmmpTTKBaseModule():
    def get_system_type(self):
        return platform.system().lower()

    def get_user(self):
        return os.getlogin()

    def get_source_directory(self):
        return os.getcwd() + "/../.."

    def get_home_directory(self):
        if self._ci_mode:
            return ""

        sys = self.get_system_type()
        if sys == "linux":
            return os.path.expanduser("~")
        elif sys == "darwin":
            return "/home/" + self.get_user()
        elif sys == "windows":
            return "D:/workspace"
        return "ttk"

    def __init__(self, mode):
        self._ci_mode = mode
        self._source_dir = self.get_source_directory() + "/qmmp-ttk/"
        self._pkg_modules = ("Input/adlmidi", "Input/adplug", "Input/openmpt", "Input/optimfrog")
        self._modules = self._pkg_modules + ("Effect/bitcrusher", "Effect/crystalizer" ,"Effect/echo", "Effect/mixer", "Effect/muffler", "Effect/silence", "Effect/sonic", "Effect/subwoofer", "Input/asap", "Input/ayfly", "Input/buzzic", "Input/dca", "Input/fc14", "Input/hively", "Input/ken", "Input/organya", "Input/psf", "Input/qoa", "Input/s98", "Input/sc68", "Input/soundmon", "Input/stsound", "Input/sunvox", "Input/tfmx", "Input/v2m", "Input/xgm", "Input/xmdx", "Input/xsf", "Visual/blurxrays", "Visual/flowwave", "Visual/infinity", "Visual/matrix", "Visual/mountain", "Visual/sonique", "Visual/voice", "Visual/volume", "Visual/vumeter")

        if not os.path.exists(self._source_dir):
            os.mkdir(self._source_dir)

