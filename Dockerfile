# syntax=docker/dockerfile:1.2

FROM pavelbezpravel/userver-demo-build-base:0.2.0 AS builder

WORKDIR /userver-demo
COPY ./ ./
RUN cmake \
    --preset=release \
    && cmake \
    --build \
    --preset=release

FROM ubuntu:22.04 AS runner
WORKDIR /userver-demo
COPY --from=builder /usr/local/bin/grpc_health_probe /usr/local/bin/grpc_health_probe
COPY --from=builder /userver-demo/cmake-build-release/userver-demo ./
COPY --from=builder /userver-demo/configs ./configs/
EXPOSE 8091 8092
ENTRYPOINT ["./userver-demo"]
CMD ["-c", "/userver-demo/configs/static_config.yaml"]
