# cpputils

[![Build Status](https://travis-ci.org/malirod/cpputils.svg?branch=master)](https://travis-ci.org/malirod/cpputils)

Different utils \ helpers in C++11. Boost is used.

## Platform

Ubuntu 18.10: Clang 7.0, GCC 8.2, Cmake 3.12, Conan

C++14 Standard is used.

See `tools/Dockerfile-dev-base` for details how to setup development environment

## Setup

Assuming all further commands are executed from project root.

### Initial setup (post clone)

Get sub-modules with the following command

`git submodule update --init --recursive`

Following clang utils are required

- clang-format (`sudo apt install clang-format`)
- clang static analyzer (`sudo apt install clang-tools`)

#### Setup git hook

Run `tools/install_hooks.py`

This will allow to perform some code checks locally before posting changes to server.

### Dependencies

Project uses [Conan Package Manager](https://github.com/conan-io/conan)

Install conan with

`sudo -H pip install conan`

Add additional repositories to conan:

- `conan remote add bincrafters https://api.bintray.com/conan/bincrafters/public-conan`

Cmake will automatically check required dependencies and setup them taking into account current compiler (clang or gcc).

Conan misses gcc 8.2 in default config at the moment. The one can use pre-defined config file.

`conan config install ./tools/conan/cfg`

## Install pylint - python checker

`sudo pip install pylint`

## Build

### Build commands

By default used clang compiler and debug mode.

Run in project root to build debug version with clang

`mkdir build-clang-debug && cd build-clang-debug && cmake .. && make -j$(nproc)`

To build release version with gcc run the following command

`RUN mkdir build-gcc-release && cd build-gcc-release && CXX=g++ cmake -DCMAKE_BUILD_TYPE=Release .. && make -j$(nproc)`

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

```bash
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

```bash
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

```bash
mkdir build-debug
cd build-debug
scan-build --use-analyzer=/usr/bin/clang++-6.0 cmake ..
scan-build --use-analyzer=/usr/bin/clang++-6.0 make -j$(nproc)
```

or

```bash
cmake ..
make clang-static-analyzer
```

### Clang-tidy

Setting are stored in `.clang-tidy`.

Run

```bash
mkdir build && cd build
cmake ..
make clang-tidy
```

### YouCompleteMe

Setup dependencies with conan and build project. Move and rename config file and additional json file to project root.

```bash
cp <build dir>/conan_ycm_extra_conf.py <project root>/.ycm_extra_conf.py
cp <build dir>/conan_ycm_flags.json <project root>/conan_ycm_flags.json
```
