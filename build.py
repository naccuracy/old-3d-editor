#!/usr/bin/env python3
#  ☀️

import sys
import os
import glob
import shutil
import platform
import subprocess
import json

#set(PROJECT_NAME awesome_project_name) from CMakeList
program_name = "2d3d"

#--------------------------------------------------utilities
def get_cpu_cores_count():
    return os.cpu_count()

def check_if_app_exists(app):
    ver_result = subprocess.run([app, '--version'])
    if ver_result.returncode:
        print("ERROR: ", app, " doesn't installed!")
        sys.exit(1)

def download_and_extract(url_str, dir_path, file_name, ar_ext):
    out_path = os.path.join(dir_path, file_name)
    ar_path = out_path + ar_ext
    if (not os.path.exists(ar_path)):
        print("DOWNLOAD: " + ar_path)
        subprocess.run(["curl", '-L', url_str, '--output', ar_path])

    if (not os.path.exists(out_path)):
        print("EXTRACT: " + ar_path)
        shutil.unpack_archive(filename=ar_path, extract_dir=dir_path, format="gztar")

def get_cmake_target():
    target = 'install'
    #if (platform.system() == "Windows"):
    #    target = 'INSTALL'
    return target

#--------------------------------------------------prepare
#check_if_app_exists("perl")
check_if_app_exists("curl")
check_if_app_exists("cmake")

ROOT_DIR = os.path.realpath(os.path.dirname(__file__))
CMAKE_GENERATOR="Unix Makefiles"
LIBSDL = "SDL2-2.28.1"
LIBIMGUI = "imgui-1.89.7"

#---------------------------------------------------
LIBS = os.path.join(ROOT_DIR, "libs")
DEST = os.path.join(ROOT_DIR, "a_builds")
LIBSBUILD=os.path.join(DEST, "libs")

if (not os.path.exists(LIBS)):
    os.mkdir(LIBS)
    os.mkdir(os.path.join(LIBS, "include"))
    os.mkdir(os.path.join(LIBS, "lib"))

if (not os.path.exists(DEST)):
    os.mkdir(DEST)

SDL_LIB_FILE = os.path.join(LIBS, "lib", "libSDL2")

SOSUFFIX = "so"
ASUFFIX = "a"
if (platform.system() == "Windows"):
#    CMAKE_GENERATOR="Visual Studio 16 2019"
    CMAKE_GENERATOR="MinGW Makefiles"
#    SOSUFFIX="dll"
#    ASUFFIX="lib"

if (platform.system() == "Darwin"):
    SOSUFFIX="dylib"

SDL_LIB_FILE += "." + ASUFFIX

if (not os.path.exists(LIBSBUILD)): 
    os.mkdir(LIBSBUILD)

TGZ=".tar.gz"
#--------------------------------------------------download libs
download_and_extract('https://github.com/libsdl-org/SDL/releases/download/release-2.28.1/' + LIBSDL + TGZ, LIBSBUILD, LIBSDL, TGZ)
download_and_extract('https://github.com/ocornut/imgui/archive/refs/tags/v1.89.7' + TGZ, LIBSBUILD, LIBIMGUI, TGZ)

#--------------------------------------------------build libs
#--------------------------------------------------SDL2
sources_dir = os.path.join(LIBSBUILD, LIBSDL)
if (os.path.exists(sources_dir)):
    print("BUILD: " + SDL_LIB_FILE)
    build_dir = sources_dir + "_build"
    if (not os.path.exists(build_dir)):
        os.mkdir(build_dir)

    subprocess.run(['cmake',
        '-G', CMAKE_GENERATOR,
        '-S', sources_dir,
        '-B', build_dir,
        '-DCMAKE_INSTALL_PREFIX=' + LIBS])
    subprocess.run(['cmake',
        '--build', build_dir,
        '--config', 'Release',
        '--target', get_cmake_target(),
        '-j{}'.format(get_cpu_cores_count())])

#--------------------------------------------------imgui no need to build, just copy sources
sources_dir = os.path.join(LIBSBUILD, LIBIMGUI)
if (os.path.exists(sources_dir)):
    target_dir = os.path.join(ROOT_DIR, 'src', 'imgui')
    if (not os.path.exists(target_dir)):
        print("COPY: " + LIBIMGUI)
        os.mkdir(target_dir)
        file_list = glob.glob(sources_dir + '/*.h')
        file_list.extend(glob.glob(sources_dir + '/*.cpp'))
        for file in file_list:
            shutil.copy(file, target_dir)
        shutil.copytree(os.path.join(sources_dir, 'backends'), os.path.join(target_dir, 'backends'))

#--------------------------------------------------build project

proj_build=os.path.join(DEST, 'project')
if (not os.path.exists(proj_build)):
    os.mkdir(proj_build)

print("BUILD: generate project for " + CMAKE_GENERATOR)
subprocess.run(['cmake',
    '-G', CMAKE_GENERATOR,
    '-S', ROOT_DIR,
    '-B', proj_build,
    '-DCMAKE_BUILD_TYPE=release'])
print("BUILD: build executable with cmake...")
subprocess.run(['cmake',
    '--build', proj_build,
    '--config','Release',
    '-j{}'.format(get_cpu_cores_count())])

print('path to project: ' + proj_build)

if (platform.system() == "Windows"):
    program_name = program_name + ".exe"
executable_path = os.path.join(proj_build, program_name)
if (os.path.exists(executable_path)):
    shutil.move(executable_path, os.path.join(ROOT_DIR, program_name))
    print('done')
    sys.exit(0)
else:
    print('ERROR: '+ executable_path + ' is not exists!')
    sys.exit(1)

