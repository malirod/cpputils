#!/usr/bin/env python
#-*- coding: utf-8 -*-

import os

def run_cmd(command):
    import subprocess
    retcode = subprocess.call(command, shell=True)
    if retcode != 0:
        raise Exception("Error while executing:\n%s" % command)

def run_cmd_with_output(command):
    import subprocess
    return subprocess.check_output(command, shell=True)

if __name__ == "__main__":
    current_script_dir = os.path.dirname(os.path.realpath(__file__))
    remote_name = "malirod-stable"
    remote_url = "https://api.bintray.com/conan/malirod/stable"

    remotes = run_cmd_with_output('conan remote list')
    if remote_name not in remotes:
        run_cmd('conan remote add {} {}'.format(remote_name, remote_url))

    conan_tools_full_path = os.path.join(current_script_dir, "recipe")
    clang_profile = os.path.join(current_script_dir, "profile-clang")
    gcc_profile = os.path.join(current_script_dir, "profile-gcc")

    # Setup gtest
    run_cmd('conan install gtest/1.8.0@malirod/stable --profile {} -r {} -f {}'.format(clang_profile, remote_name, os.path.join(conan_tools_full_path, "conan-gtest")))
    run_cmd('conan install gtest/1.8.0@malirod/stable --profile {} -r {} -f {}'.format(gcc_profile, remote_name, os.path.join(conan_tools_full_path, "conan-gtest")))

    # Setup log4cplus
    run_cmd('conan install log4cplus/2.0.0-RC2@malirod/stable --profile {} -r {} -f {}'.format(clang_profile, remote_name, os.path.join(conan_tools_full_path, "conan-log4cplus")))
    run_cmd('conan install log4cplus/2.0.0-RC2@malirod/stable --profile {} -r {} -f {}'.format(gcc_profile, remote_name, os.path.join(conan_tools_full_path, "conan-log4cplus")))
