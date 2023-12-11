# syntax=docker/dockerfile:1.2

FROM ubuntu:22.04 AS builder

RUN apt-get update && DEBIAN_FRONTEND=nointeractive apt-get install -y --no-install-recommends \
    clangd-14 \
    clang-14 \
    cmake \
    curl \
    gcc \
    gdb \
    git \
    g++ \
    iwyu \
    lld-14 \
    lldb-14 \
    llvm-14 \
    make \
    m4 \
    ninja-build \
    pkg-config \
    python3 \
    python3-dev \
    python3-jinja2 \
    python3-pip \
    python3-protobuf \
    python3-virtualenv \
    python3-voluptuous \
    sudo \
    tzdata \
    xz-utils \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/* \
    && pip3 install --no-cache-dir conan==1.62.0

WORKDIR /grpc_health_probe
RUN curl -L https://github.com/grpc-ecosystem/grpc-health-probe/releases/download/v0.4.23/grpc_health_probe-linux-amd64 -o grpc_health_probe \
    && install -o root -g root -m 0755 grpc_health_probe /usr/local/bin/grpc_health_probe

WORKDIR /ccache
RUN export V=4.8.1 \
    && curl -Lo ccache-${V}-linux-x86_64.tar.xz https://github.com/ccache/ccache/releases/download/v${V}/ccache-${V}-linux-x86_64.tar.xz \
    && tar -Jxvf ccache-${V}-linux-x86_64.tar.xz -C ./ --strip-components=1 --no-same-owner ccache-${V}-linux-x86_64/ccache \
    && rm -rf ccache-${V}-linux-x86_64.tar.xz \
    && unset V \
    && install -o root -g root -m 0755 ./ccache /usr/local/bin/ccache

# https://code.visualstudio.com/remote/advancedcontainers/add-nonroot-user#_creating-a-nonroot-user

ARG USERNAME=builder
ARG USER_UID=1000
ARG USER_GID=$USER_UID

RUN groupadd --gid $USER_GID $USERNAME \
    && useradd --uid $USER_UID --gid $USER_GID -m -s /bin/bash $USERNAME \
    && echo $USERNAME ALL=\(root\) NOPASSWD:ALL > /etc/sudoers.d/$USERNAME \
    && chmod 0440 /etc/sudoers.d/$USERNAME

USER $USERNAME

WORKDIR /conan
RUN --mount=type=bind,source=./conan,target=./ \
    conan config install .

# INFO [pavelbezpravel]:
# We copy the existing third_party/userver instead of cloning the repository,
# because in this case we can manually checkout to commits that are safe and reliable.
# I hope userver will be added to the conan-center-index soon.

WORKDIR /userver-framework
RUN --mount=type=bind,source=./third_party/userver,target=./userver \
    conan create \
    /userver-framework/userver \
    --build=missing \
    -pr=userver-demo/default \
    -pr:b=userver-demo/default \
    -pr:h=userver-demo/default \
    -s build_type=Release \
    -s:b build_type=Release \
    -s:h build_type=Release \
    && conan create \
    /userver-framework/userver \
    --build=missing \
    -pr=userver-demo/default \
    -pr:b=userver-demo/default \
    -pr:h=userver-demo/default \
    -s build_type=Debug \
    -s:b build_type=Debug \
    -s:h build_type=Debug \
    && conan remove "*" --builds --src --force
WORKDIR /home/builder
EXPOSE 8091 8092
