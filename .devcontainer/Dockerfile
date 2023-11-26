# syntax=docker/dockerfile:1.2

FROM ubuntu:22.04 AS builder

RUN apt-get update && DEBIAN_FRONTEND=nointeractive apt-get install -y --no-install-recommends \
    ccache \
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
    tzdata \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/* \
    && pip3 install --no-cache-dir conan==1.62.0

RUN groupadd builder \
    && useradd -m -s /bin/bash -g builder builder
USER builder:builder

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
    && conan remove "*" --builds --src --force
WORKDIR /home/builder
EXPOSE 8091 8092
CMD ["tail", "-f", "/dev/null"]
