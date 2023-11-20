# userver-demo

## Requirements
- docker 24.0.7
- docker compose 2.21.0
- devcontainers/cli (optional)

## Build

### Docker

You can build a project manually with `docker`. You have to build an image and run a containers with `cmake` commands:

```bash
mkdir -p cmake_build
docker build -t userver-demo/dev -f .devcontainer/Dockerfile .

docker run \
  --rm \
  -it \
  -u 1000:1000 \
  -v "$(pwd)":/workspaces/userver-demo \
  userver-demo/dev \
  cmake \
  -DCMAKE_BUILD_TYPE=Release \
  -G Ninja \
  -DCMAKE_TOOLCHAIN_FILE=/workspaces/userver-demo/clang.toolchain \
  -S /workspaces/userver-demo \
  -B /workspaces/userver-demo/cmake_build

docker run \
  --rm \
  -it \
  -u 1000:1000 \
  -v "$(pwd)":/workspaces/userver-demo \
  userver-demo/dev \
  cmake \
  --build /workspaces/userver-demo/cmake_build
```

### Dev Container Cli

Also, you can build a project with [devcontainers](https://containers.dev/) in an easy and convenient way.
Your IDE (e.g. Clion) or code editor (e.g. VS Code) can run and attach to devcontainer.
Install devcontainers/cli with npm:

```bash
npm install -g @devcontainers/cli
```

Run devcontainer and commands inside it:
```bash
devcontainer up --workspace-folder .

devcontainer exec --workspace-folder . \
  cmake \
  -DCMAKE_BUILD_TYPE=Release \
  -G Ninja \
  -DCMAKE_TOOLCHAIN_FILE=/workspaces/userver-demo/clang.toolchain \
  -S /workspaces/userver-demo \
  -B /workspaces/userver-demo/cmake_build

devcontainer exec --workspace-folder . \
  cmake \
  --build /workspaces/userver-demo/cmake_build
```

## Run

```bash
docker compose up --build
```
