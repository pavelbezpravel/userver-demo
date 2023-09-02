# userver-demo

## Build

```bash
mkdir -p cmake_build
docker build -t userver-demo -f Dockerfile .

docker run \
  --rm \
  -it \
  -u 1000:1000 \
  -v $(pwd):/tmp/userver-demo \
  -v $(conan config home):/home/builder/.conan \
  userver-demo \
  cmake -DCMAKE_BUILD_TYPE=Release \
  -G Ninja \
  -DCMAKE_TOOLCHAIN_FILE=/tmp/userver-demo/clang.toolchain \
  -S /tmp/userver-demo \
  -B /tmp/userver-demo/cmake_build

docker run \
  --rm \
  -it \
  -u 1000:1000 \
  -v $(pwd):/tmp/userver-demo \
  -v $(conan config home):/home/builder/.conan \
  userver-demo \
  cmake \
  --build /tmp/userver-demo/cmake_build
```
