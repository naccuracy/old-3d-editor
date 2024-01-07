#!/usr/bin/env python3
#  ☀️

import sys
import os
import shutil

def rm_rf_if_exists(dir):
    if (os.path.exists(dir)):
        shutil.rmtree(dir)

root_dir = os.path.realpath(os.path.dirname(__file__))
#---------------------------------------------------
LIBS=os.path.join(root_dir, "libs")
DEST=os.path.join(root_dir, "a_builds")
LIBSBUILD=os.path.join(DEST, "libs")
LIBSDL=os.path.join(LIBSBUILD, "SDL2-2.28.1")

rm_rf_if_exists(LIBS)
rm_rf_if_exists(os.path.join(DEST, "project"))
rm_rf_if_exists(LIBSDL)
rm_rf_if_exists(LIBSDL + "_build")

print("done")
