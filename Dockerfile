# syntax=docker/dockerfile:1

# docker build -t userver-demo -f Dockerfile .

FROM ghcr.io/userver-framework/ubuntu-userver-build-base:v1

RUN apt-get update && apt-get install -y \
    clang-14 \
    cmake \
    gdb \
    git \
    lld-14 \
    lldb-14 \
    llvm-14 \
    make \
    ninja-build \
    pkg-config \
    python3 \
    python3-pip \
    valgrind

RUN pip3 install conan==1.59.0

ARG UID=1000
RUN useradd -m -u ${UID} -s /bin/bash builder
