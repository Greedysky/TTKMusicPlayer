#!/usr/bin/env python
# -*- coding: utf-8 -*-
#Author : Greedysky

import os
import sys

def get_file_prefix(filename):
    return filename.split(".")[0]

def get_file_suffix(filename):
    return filename[filename.rfind(".") + 1:]

def translate(lcexe, output, source):
    output_path = output + "/GLanguage"
    source_path = source + "/TTKLanguage"

    # make output dir
    if not os.path.exists(output_path):
        os.makedirs(output_path)

    for f in os.listdir(source_path):
        if get_file_suffix(f) != "ts":
            continue

        # make lrelease
        os.system("%s %s/%s" % (lcexe, source_path, f))
        # rename and move 
        file_prefix = get_file_prefix(f)
        os.rename("%s/%s.qm" % (source_path, file_prefix), "%s/%s.ln" % (output_path, file_prefix))

if __name__ == "__main__":

    argc = len(sys.argv)
    if argc == 4:
        translate(sys.argv[1], sys.argv[2], sys.argv[3])

    print ("translate finished!")
    print ("=============================================================================")
