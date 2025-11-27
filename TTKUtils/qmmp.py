#!/usr/bin/env python
# -*- coding: utf-8 -*-
#Author : Greedysky

import os
import shutil
import platform
import argparse

def _get_system_type():
    return platform.system().lower()

def _copy_dir(src, dst):
    if not os.path.exists(dst):
        shutil.copytree(src, dst)
        return;

    for f in os.listdir(src):
        s = os.path.join(src, f)
        d = os.path.join(dst, f)

        if os.path.isfile(s):
            shutil.copyfile(s, d)
        else:
            _copy_dir(s, d)


class QmmpCopyModule():
    def __init__(self, src, dst, ver):
        self._src_directory = src
        self._dst_directory = dst
        self._current_version = ver
        self._win_src_verions = ("4_8_7", "5_5_1", "6_2_4")
        self._versions = ("4_8_7", "5_0_2", "5_1_1", "5_2_1", "5_3_2", "5_4_2", "5_5_1", "5_6_3", "5_7_1", "5_8_0", "5_9_9", "5_10_1", "5_11_3", "5_12_12", "5_13_2", "5_14_2", "5_15_2" if not self.is_windows() else "5_15_18", "6_0_4", "6_1_3", "6_2_4", "6_3_2", "6_4_3", "6_5_3", "6_6_3", "6_7_3", "6_8_3", "6_9_3", "6_10_1")

    def is_windows(self):
        return _get_system_type() == "windows"

    def make_bin_path(self, path, name):
        return os.path.join(path, os.path.join(name, "bin"))

    def run(self):
        src_dirs = os.listdir(self._src_directory)
        dst_dirs = os.listdir(self._dst_directory)
        app_name = "demo.exe" if self.is_windows() else "demo"

        # find all supported versions
        for version in self._versions:
            dst_directory = ""
            src_directory = ""

            # find desetination dir names
            for dir in dst_dirs:
                if version in dir:
                    dst_directory = self.make_bin_path(self._dst_directory, dir)
                    break

            # find win src versions
            if self.is_windows():
                for dir in src_dirs:
                    if version not in dir:
                        continue

                    if version.startswith("4"):
                        version = self._win_src_verions[0]
                        break
                    elif version.startswith("5"):
                        version = self._win_src_verions[1]
                        break
                    elif version.startswith("6"):
                        version = self._win_src_verions[2]
                        break

            # find source dir names
            for dir in src_dirs:
                if version in dir:
                    src_directory = self.make_bin_path(self._src_directory, dir)
                    break

            if src_directory and dst_directory:
                _copy_dir(src_directory, dst_directory)
                # remove executable file path
                os.remove(os.path.join(dst_directory, os.path.join(self._current_version, app_name)))

if __name__ == "__main__":

    parser = argparse.ArgumentParser()
    parser.add_argument('-s', help = "源构建目录")
    parser.add_argument('-d', help = "目标构建目录")
    parser.add_argument('-v', help = "TTK版本")

    args = parser.parse_args()
    print ("setup copy start!")

    module = QmmpCopyModule(args.s, args.d, args.v)
    module.run()

    print ("setup copy finished!")
    print ("=============================================================================")
