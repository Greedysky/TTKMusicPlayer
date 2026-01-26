#!/usr/bin/env python
# -*- coding: utf-8 -*-
#Author : Greedysky

import os

from qmmp_ttk_base import QmmpTTKBaseModule

class BuildModule(QmmpTTKBaseModule):
    def __init__(self): 
        QmmpTTKBaseModule.__init__(self)

    def run(self):
        rm_str = "rm .qmake.stash Makefile *.so"
        # 引入Qmmp环境配置
        os.environ['PKG_CONFIG_PATH'] = self.get_source_directory() + "/qmmp-lib/qt5/lib/qmmp/pkgconfig"
        # 遍历所有插件模块
        for module in self._modules:
            print ("Qmmp plugin module now is " + module)
            # 进入插件子目录
            os.chdir(self._source_dir + module)
            # 编译
            os.system(self.get_home_directory() + "/Qt/5.9.9/gcc_64/bin/qmake")
            status = os.system("make -j6")
            if status != 0:
                os.system(rm_str)
                exit()
            # 安装
            os.system("make install")
            # 清除缓存
            os.system("make clean")
            os.system(rm_str)
            print ("////////////////////////////////////////////////\n")

if __name__ == "__main__":

    module = BuildModule()
    module.run()

