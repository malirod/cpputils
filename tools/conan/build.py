#!/usr/bin/env python
#-*- coding: utf-8 -*-

import os
import contextlib

def run_cmd(command):
    import subprocess
    retcode = subprocess.call(command, shell=True)
    if retcode != 0:
        raise Exception("Error while executing:\n%s" % command)

def run_cmd_with_output(command):
    import subprocess
    return subprocess.check_output(command, shell=True)

@contextlib.contextmanager
def working_directory(path):
    """ A context manager which changes the working directory to the given
    path, and then changes it back to it's previous value on exit.
    """
    prev_cwd = os.getcwd()
    os.chdir(path)
    try:
        yield
    finally:
        os.chdir(prev_cwd)

if __name__ == "__main__":
    CURRENT_SCRIPT_DIR = os.path.dirname(os.path.realpath(__file__))
    REMOTE_NAME = "malirod-stable"
    REMOTE_URL = "https://api.bintray.com/conan/malirod/stable"

    REMOTES = run_cmd_with_output('conan remote list')
    if REMOTE_NAME not in REMOTES:
        run_cmd('conan remote add {} {}'.format(REMOTE_NAME, REMOTE_URL))

    CONAN_TOOLS_FULL_PATH = os.path.join(CURRENT_SCRIPT_DIR, "recipe")
    CLANG_PROFILE = os.path.join(CURRENT_SCRIPT_DIR, "profile-clang")
    GCC_PROFILE = os.path.join(CURRENT_SCRIPT_DIR, "profile-gcc")

    # Setup gtest
    with working_directory(os.path.join(CONAN_TOOLS_FULL_PATH, "conan-gtest")):
        run_cmd('conan install gtest/1.8.0@malirod/stable --profile {} --build missing'.format(CLANG_PROFILE))
        run_cmd('conan install gtest/1.8.0@malirod/stable --profile {} --build missing'.format(GCC_PROFILE))

    # Setup log4cplus
    with working_directory(os.path.join(CONAN_TOOLS_FULL_PATH, "conan-log4cplus")):
        run_cmd('conan install log4cplus/2.0.0-RC2@malirod/stable --profile {} --build missing'.format(CLANG_PROFILE))
        run_cmd('conan install log4cplus/2.0.0-RC2@malirod/stable --profile {} --build missing'.format(GCC_PROFILE))

    # Setup Boost
    with working_directory(os.path.join(CONAN_TOOLS_FULL_PATH, "conan-boost")):
        run_cmd('conan install Boost/1.66.0@malirod/stable --profile {} --build missing'.format(CLANG_PROFILE))
        run_cmd('conan install Boost/1.66.0@malirod/stable --profile {} --build missing'.format(GCC_PROFILE))
