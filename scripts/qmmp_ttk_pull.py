#!/usr/bin/env python
# -*- coding: utf-8 -*-
#Author : Greedysky

import os

from qmmp_ttk_base import QmmpTTKBaseModule

class GitPullModule(QmmpTTKBaseModule):
    def __init__(self): 
        QmmpTTKBaseModule.__init__(self)

    def run(self):
        # 遍历所有插件模块
        for module in self._modules:
            print ("Qmmp plugin module now is " + module)
            # 进入插件子目录
            os.chdir(self._source_dir + module)
            # 拉取代码
            status = os.system("git pull")
            if status != 0:
                exit(1)
            print ("////////////////////////////////////////////////\n")

if __name__ == "__main__":

    module = GitPullModule()
    module.run()

