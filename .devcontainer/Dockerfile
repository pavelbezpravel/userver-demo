# syntax=docker/dockerfile:1

FROM ubuntu:22.04 AS builder

RUN apt-get update && DEBIAN_FRONTEND=nointeractive apt-get install -y \
    tzdata \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*

RUN apt-get update && apt-get install -y \
    ccache \
    clangd-14 \
    clang-14 \
    cmake \
    curl \
    gcc \
    gdb \
    git \
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
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/* \
    && pip3 install conan==1.62.0

ARG UID=1000
SHELL ["/bin/bash", "-o", "pipefail", "-c"]
RUN useradd -m -u ${UID} -s /bin/bash builder \
    && echo "builder:builder" | chpasswd \
    && adduser builder sudo \
    && echo '%sudo ALL=(ALL) NOPASSWD:ALL' >> /etc/sudoers
USER builder

WORKDIR /conan
COPY ./conan ./
RUN conan config install .

# INFO [pavelbezpravel]:
# We copy the existing third_party/userver instead of cloning the repository,
# because in this case we can manually checkout to commits that are safe and reliable.
# I hope userver will be added to the conan-center-index soon.

WORKDIR /userver-framework
COPY --chown=builder:builder third_party/userver ./userver
RUN conan create \
    /userver-framework/userver \
    --build=missing \
    -pr=userver-demo/default \
    -pr:b=userver-demo/default \
    -pr:h=userver-demo/default \
    && conan remove "*" --builds --src --force \
    && rm -rf /userver-framework/userver
WORKDIR /home/builder
EXPOSE 8091 8092
