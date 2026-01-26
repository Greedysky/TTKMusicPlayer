#!/usr/bin/env python
# -*- coding: utf-8 -*-
#Author : Greedysky

import os

from qmmp_ttk_base import QmmpTTKBaseModule

class BuildModule(QmmpTTKBaseModule):
    def __init__(self): 
        QmmpTTKBaseModule.__init__(self)

    def run(self):
        with open("build.sh", "w") as file:
            file.write("#!/bin/bash\n\n")
            file.write("modules=(%s)\n\n" % " ".join(self._modules))
            file.write('for module in "${modules[@]}"; do\n')
            file.write('  echo "Qmmp plugin module now is "$module\n')
            file.write("  cd /home/runner/work/TTKMusicPlayer/qmmp-ttk/$module\n")
            file.write("  qmake\n")
            file.write("  make -j6\n")
            file.write("  make clean\n")
            file.write("  rm .qmake.stash Makefile *.so\n")
            file.write("done\n")

        print ("////////////////////////////////////////////////\n")

if __name__ == "__main__":

    module = BuildModule()
    module.run()

