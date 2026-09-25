# Building

Requirements: Qt 6.6+ (Widgets, Concurrent), CMake 3.25+, Ninja, a C++20 compiler
(Xcode command line tools).

```sh
brew install qt cmake ninja
```

## Command line

```sh
cmake --preset debug            # configure once (again only if CMakeLists.txt changes)
cmake --build --preset run      # build and start the app, log in this terminal
```

`cmake --build --preset debug` only builds. The result is an app bundle,
`build/debug/bsc.app`, which is a folder: start it with `open build/debug/bsc.app`
or run the executable inside it, `build/debug/bsc.app/Contents/MacOS/bsc`.
Release: `cmake --preset release && cmake --build --preset release`.

The presets (`CMakePresets.json`) expect Homebrew's Qt in `/opt/homebrew/opt/qt`.
For another Qt install, set `QT_PREFIX` in a `CMakeUserPresets.json` (not committed):

```json
{
  "version": 6,
  "configurePresets": [
    { "name": "my-debug", "inherits": "debug",
      "environment": { "QT_PREFIX": "$env{HOME}/Qt/6.x/macos" } }
  ]
}
```

## VS Code

1. Open the folder and install the recommended extensions
   (CMake Tools, C/C++, CodeLLDB).
2. Pick the **Debug** or **Release** preset in the CMake status bar / panel.
3. Build: **⇧⌘B**. Run without debugger: *Terminal → Run Task → Run*.
   Debug: **F5** (“Debug bsc”, builds first).

IntelliSense uses the CMake Tools configuration (`compile_commands.json`).

## Qt Creator

Open `CMakeLists.txt`; the presets are picked up automatically.

## Universal binary / distribution

A universal binary (Apple Silicon + Intel) needs a Qt built for both
architectures (e.g. from the Qt online installer):

```sh
cmake --preset release -DCMAKE_OSX_ARCHITECTURES="arm64;x86_64"
```

Self-contained bundle: `macdeployqt build/release/bsc.app -dmg`.
