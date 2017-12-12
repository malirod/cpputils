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

You can enable sanitizers with `SANITIZE_ADDRESS`, `SANITIZE_MEMORY`, `SANITIZE_THREAD` or `SANITIZE_UNDEFINED` options in your CMake configuration. You can do this by passing e.g. `-DSANITIZE_ADDRESS=On` in your command line.

## Run

Run from project root. It's expected that config is located in the project root.

`build/debug/testrunner`

## Coverage report

To enable coverage support in general, you have to enable `ENABLE_COVERAGE` option in your CMake configuration. You can do this by passing `-DENABLE_COVERAGE=On` on your command line or with your graphical interface.

If coverage is supported by your compiler, the specified targets will be build with coverage support. If your compiler has no coverage capabilities (I asume intel compiler doesn't) you'll get a warning but CMake will continue processing and coverage will simply just be ignored.

Collect coverage in Debug mode. Tested with gcc compiler. Clang known to be supported too.

### Sample commands to get coverage html report

```
CXX=g++ cmake -DENABLE_COVERAGE=On -DCMAKE_BUILD_TYPE=Release ..
make -j$(nproc)
make test
make testrunner-geninfo
make testrunner-genhtml
xdg-open lcov/html/testrunner/index.html
```

## Integration

`Dockerfile` creates build environment from the scratch. It should be built manually and pushed to DockerHub

`Dockerfile-travis` is used by Travis. It's based on pre-built image from `Dockerfile` on DockerHub

### Create docker image

**Dockerfile-dev-base**: base image, which contains basic env setup (compiler, build tools)

`docker build -t cpp-dev-base -f tools/Dockerfile-dev-base .`

**Dockerfile-initial**: initial project image, which contains pre-build sources. Based on **Dockerfile-dev-base**

`docker build -t travis-build-cpputils -f tools/Dockerfile-initial .`

Steps to prepare image for Travis

```
docker build -t cpp-dev-base -f tools/Dockerfile-dev-base .
docker build -t travis-build-cpputils -f tools/Dockerfile-initial .
docker login
docker tag cpp-dev-base $DOCKER_ID_USER/cpp-dev-base
docker tag travis-build-cpputils $DOCKER_ID_USER/dev-cpputils
docker push $DOCKER_ID_USER/cpp-dev-base
docker push $DOCKER_ID_USER/dev-cpputils
```
