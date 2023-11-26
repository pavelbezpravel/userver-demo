# syntax=docker/dockerfile:1.2

FROM pavelbezpravel/userver-demo-build-base:0.1.0 AS builder

WORKDIR /userver-demo
COPY ./ ./
RUN cmake \
    -DCMAKE_BUILD_TYPE=Release \
    -G Ninja \
    -DCMAKE_TOOLCHAIN_FILE=clang.toolchain \
    -S /userver-demo \
    -B /userver-demo/cmake_build \
    && cmake \
    --build /userver-demo/cmake_build \
    -j "$(nproc)"

FROM ubuntu:22.04 AS runner
WORKDIR /userver-demo
COPY --from=builder /userver-demo/cmake_build/userver-demo ./
COPY --from=builder /userver-demo/configs ./configs/
EXPOSE 8091 8092
ENTRYPOINT ["./userver-demo"]
CMD ["-c", "/userver-demo/configs/static_config.yaml"]
