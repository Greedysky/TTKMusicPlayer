#!/usr/bin/env python
# -*- coding: utf-8 -*-
#Author : Greedysky

import os

from qmmp_ttk_base import QmmpTTKBaseModule

class GitCloneModule(QmmpTTKBaseModule):
    def __init__(self): 
        QmmpTTKBaseModule.__init__(self)

    def run(self):
        # 遍历所有插件模块
        for module in self._modules:
            print ("Qmmp plugin module now is " + module)
            # 进入主目录
            os.chdir(self._source_dir)
            # 克隆代码
            module_dir = module.split("/")[0]
            module_name = module.split("/")[1]

            if not os.path.exists(module_dir):
                os.mkdir(module_dir)

            # 进入插件子目录
            os.chdir(self._source_dir + module_dir)

            status = os.system("git clone https://github.com/TTK-qmmp/qmmp-%s.git %s" % (module_name, module_name))
            if status != 0:
                exit(1)
            print ("////////////////////////////////////////////////\n")

if __name__ == "__main__":

    module = GitCloneModule()
    module.run()

