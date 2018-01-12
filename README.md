[![Build Status](https://travis-ci.org/malirod/cpputils.svg?branch=master)](https://travis-ci.org/malirod/cpputils)

# cpputils
Different utils \ helpers in C++11. Boost is used.

## Platform

Ubuntu 16.04: Clang 5.0, GCC 5.4, Cmake 3.5, Conan

C++11 Standard is used.

See `tools/Dockerfile-dev-base` for details how to setup development environment

## Setup

Assuming all further commands are executed from project root.

### Initial setup (post clone)

Get sub-modules with the following command

`git submodule update --init --recursive`

#### Setup git hook

Run `tools/install_hooks.py`

This will allow to perform some code checks locally before posting changes to server.

### Dependencies

Project uses [Conan Package Manager](https://github.com/conan-io/conan)

Install conan with

`sudo -H pip install conan`

To get dependencies from remote repository run command in project root

`tools/conan/build.py`

Before calling cmake to generate build files generate cmake files for dependencies (from build dir)

`conan install .. --profile ../tools/conan/profile-clang`

**Hint:** to upload build packages to server use the following commands

```
conan remote add <REMOTE> https://api.bintray.com/conan/malirod/stable
conan user -p <APIKEY> -r <REMOTE> <USERNAME>
conan install . -r <REMOTE>
conan upload "*" -r <REMOTE> --all
```

## Install pylint - python checker

`sudo pip install pylint==1.8.0`

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

Run from build directory

`ctest`

or

`bin/testrunner`

## Coverage report

To enable coverage support in general, you have to enable `ENABLE_COVERAGE` option in your CMake configuration. You can do this by passing `-DENABLE_COVERAGE=On` on your command line or with your graphical interface.

If coverage is supported by your compiler, the specified targets will be build with coverage support. If your compiler has no coverage capabilities (I assume Intel compiler doesn't) you'll get a warning but CMake will continue processing and coverage will simply just be ignored.

Collect coverage in Debug mode. Tested with gcc 5.0 and clang 5.0 compiler.

### Sample commands to get coverage html report

```
CXX=g++ cmake -DENABLE_COVERAGE=On -DCMAKE_BUILD_TYPE=Debug ..
make -j$(nproc)
make test
make testrunner-genhtml
xdg-open lcov/html/testrunner/index.html
```

## Integration

`Dockerfile-initial` creates build environment from the scratch. It should be built manually and pushed to DockerHub

`Dockerfile-travis` is used by Travis. It's based on pre-built image from `Dockerfile-initial` on DockerHub

### Create docker image

**Dockerfile-dev-base**: base image, which contains basic environment setup (compiler, build tools)

`docker build -t cpp-dev-base -f tools/Dockerfile-dev-base .`

**Dockerfile-initial**: initial project image, which contains pre-build sources. Based on **Dockerfile-dev-base**

`docker build -t travis-build-cpputils -f tools/Dockerfile-initial .`

Steps to prepare image for Travis

```
docker build -t cpp-dev-base -f tools/Dockerfile-dev-base .
docker tag cpp-dev-base $DOCKER_ID_USER/cpp-dev-base
docker build -t travis-build-cpputils -f tools/Dockerfile-initial .
docker tag travis-build-cpputils $DOCKER_ID_USER/dev-cpputils
docker login
docker push $DOCKER_ID_USER/cpp-dev-base
docker push $DOCKER_ID_USER/dev-cpputils
```
### Clang static analyzer

Sample command to run analyzer. By default report is stored in `/tmp/scan-build*`

```
mkdir build-debug
cd build-debug
scan-build --use-analyzer=/usr/bin/clang++-5.0 cmake ..
scan-build --use-analyzer=/usr/bin/clang++-5.0 make -j$(nproc)
```

or


```
cmake ..
make clang-static-analyzer
```

### Clang-tidy

Setting are stored in `.clang-tidy`.

Run

```
mkdir build
cd build
cmake ..
make clang-tidy
```

### Include-What-You-Use

Setup for CLang 5.0

Prepare IWYU

```
sudo apt install libncurses5-dev libclang-5.0-dev libz-dev
git clone https://github.com/include-what-you-use/include-what-you-use.git
git checkout -b clang_5.0 origin/clang_5.0
mkdir build && cd build
cmake -DIWYU_LLVM_ROOT_PATH=/usr/lib/llvm-5.0 -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ ..
make
sudo make install
```

Once `include-what-you-use` is availabe in the `PATH` the one can check project by invoking `make iwyu`.
