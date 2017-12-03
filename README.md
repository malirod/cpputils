[![Build Status](https://travis-ci.org/malirod/cpputils.svg?branch=master)](https://travis-ci.org/malirod/cpputils)

# cpputils
Dirrerent utils \ helpers in C++11. Boost is used.

## Platform

Ubuntu 16.04: Clang 5.0, GCC 5.4

C++11 Standard is used.

## Setup

### Initial setup (post clone)

Get sub-modules with the following command

`git submodule update --init --recursive`

### Install Boost

#### Use pre-build Boost lib

Use the following repository: https://github.com/malirod/boost-bin

Use appropriate branch. Branch name contains info about used env and options to build Boost.

E.g.

https://github.com/malirod/boost-bin/tree/boost-1.62-ubuntu-16.04-x64-clang-5.8-release-c++11-static-multi

#### Build Boost manually from sources
If pre-build version doesn't fit your needs (OS, compiler etc) then build Boost manually.

Download and unzip to some dir (e.g. `~/libs/boost_1_62_0`) latest stable version from [Boost site](http://www.boost.org/).

Go the unzipped directory and run commands

```
./bootstrap.sh --prefix=./build
./b2 --toolset=clang cxxflags="-std=c++11" variant=release link=static threading=multi -j$(nproc) install

```

#### Set environment variable
Set env pointing to the boost install dir (in ~/.profile or ~/.bashrc)

`export BOOST_ROOT=~/libs/boost_1_62_0/build`

Restart terminal, or reload config with `source ~/.profile` (`source ~/.bashrc`)

## Setup git hook

Run `python tools/install_hooks.py`

This will allow to perform some code checks locally before posting changes to server.

## Install pylint - python checker

`sudo apt-get install pylint`

## Logger

Project uses log4cplus logger. It's configured as the sub-module located in `thirdparty\log4cplus`.

## Build

#### Build commands

By default used clang compiler and debug mode.

Run in project root to build debug version with clang

`mkdir build-clang-debug && cd build-clang-debug && cmake .. && make -j$(nproc)`

To build release version with gcc run the follwing command

`RUN mkdir build-gcc-release && cd build-gcc-release && cmake -DCMAKE_CXX_COMPILER=g++ -DCMAKE_BUILD_TYPE=Release .. && make -j$(nproc)`

### Build with sanitizers (clang)

Use the following option for configuration. Applicable to debug build only. Known values are:

TBD

## Run

Run from project root. It's expected that config is located in the project root.

`build/debug/testrunner`

## Integration

`Dockerfile` creates build environment from the scratch. It should be built manually and pushed to DockerHub

`Dockerfile-travis` is used by Travis. It's based on pre-built image from `Dockerfile` on DockerHub

### Create docker image

Steps to prepare image for Travis

```
docker build -t travis-build-cpputils -f tools/Dockerfile .
docker login
docker tag travis-build-cpputils $DOCKER_ID_USER/dev-cpputils
docker push $DOCKER_ID_USER/dev-cpputils
```
