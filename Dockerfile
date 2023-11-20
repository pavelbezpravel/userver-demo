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
    git \
    lld-14 \
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
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/* \
    && pip3 install conan==1.62.0

WORKDIR /conan
COPY ./conan ./
RUN conan config install .

# INFO [pavelbezpravel]:
# We copy the existing third_party/userver instead of cloning the repository,
# because in this case we can manually checkout to commits that are safe and reliable.
# I hope userver will be added to the conan-center-index soon.

WORKDIR /userver-framework
COPY third_party/userver ./userver
RUN conan create \
    /userver-framework/userver \
    --build=missing \
    -pr=userver-demo/default \
    -pr:b=userver-demo/default \
    -pr:h=userver-demo/default \
    && conan remove "*" --builds --src --force \
    && rm -rf /userver-framework/userver

WORKDIR /userver-demo
COPY ./ ./

RUN cmake \
    -DCMAKE_BUILD_TYPE=Release \
    -G Ninja \
    -DCMAKE_TOOLCHAIN_FILE=clang.toolchain \
    -S . \
    -B cmake_build \
    && cmake \
    --build cmake_build \
    -j "$(nproc)"

FROM ubuntu:22.04 AS runner
WORKDIR /userver-demo
COPY --from=builder /userver-demo/cmake_build/userver-demo ./
COPY --from=builder /userver-demo/configs ./configs/
EXPOSE 8091 8092
ENTRYPOINT ["./userver-demo"]
CMD ["-c", "/userver-demo/configs/static_config.yaml"]
