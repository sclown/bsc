# Building

Requirements: Qt 6.6+ (Widgets, Concurrent), CMake 3.21+, a C++20 compiler.

## macOS (Homebrew)

```sh
brew install qt cmake ninja
cmake -S . -B build -G Ninja -DCMAKE_PREFIX_PATH="$(brew --prefix qt)" -DCMAKE_BUILD_TYPE=Release
cmake --build build
open build/bsc.app
```

Universal binary (Apple Silicon + Intel) requires a Qt built for both
architectures (e.g. from the Qt online installer):

```sh
cmake -S . -B build -G Ninja -DCMAKE_PREFIX_PATH=~/Qt/6.x/macos \
      -DCMAKE_OSX_ARCHITECTURES="arm64;x86_64"
```

To make a self-contained bundle: `macdeployqt build/bsc.app -dmg`.

Qt Creator: open `CMakeLists.txt` directly.
