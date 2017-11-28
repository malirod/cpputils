FROM ubuntu:16.04

ENV HOME /root
ENV PROJECT_ROOT $HOME/project
ENV BOOST_BIN $HOME/libs/boost-bin
ENV BOOST_BRANCH boost-1.62-ubuntu-16.04-x64-clang-5.0-release-c++11-static-multi
ENV BOOST_HOME $BOOST_BIN/build

ADD . $PROJECT_ROOT

WORKDIR $HOME

RUN apt-get update
RUN apt-get install -y wget
RUN wget -O - https://apt.llvm.org/llvm-snapshot.gpg.key|apt-key add -
RUN echo "deb http://apt.llvm.org/xenial/ llvm-toolchain-xenial-5.0 main" | tee -a /etc/apt/sources.list
RUN echo "deb-src http://apt.llvm.org/xenial/ llvm-toolchain-xenial-5.0 main" | tee -a /etc/apt/sources.list
RUN apt-get update
RUN apt-get install -y git
RUN apt-get install -y clang-5.0 clang-format-5.0 g++ make
RUN apt-get install -y pylint

RUN update-alternatives --install /usr/bin/clang++ clang++ /usr/bin/clang++-5.0 50
RUN update-alternatives --install /usr/bin/clang clang /usr/bin/clang-5.0 50
RUN update-alternatives --install /usr/bin/llvm-symbolizer llvm-symbolizer /usr/bin/llvm-symbolizer-5.0 50
RUN update-alternatives --install /usr/bin/clang-format clang-format /usr/bin/clang-format-5.0 50

RUN git clone https://github.com/malirod/boost-bin.git $BOOST_BIN
WORKDIR $BOOST_BIN
RUN git fetch
RUN git checkout ${BOOST_BRANCH}
RUN tar -xzf build.tar.gz

WORKDIR $PROJECT_ROOT

# Preconfigure thirdparties with Clang
RUN ./waf configure
# Preconfigure thirdparties with Gcc
RUN ./waf configure --check-cxx-compiler=g++
