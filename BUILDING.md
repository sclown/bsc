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
4. Preset-based alternatives that do not depend on the CMake Tools
   selection: tasks *Build (debug preset)*, *Run (debug preset)*,
   *Build (release preset)*, and the launch configurations
   *Debug bsc (debug preset)* / *Debug bsc (release preset)*.

IntelliSense uses the CMake Tools configuration (`compile_commands.json`).

## Qt Creator

Open `CMakeLists.txt`; the presets are picked up automatically.

## Linux and Windows

The code is portable; only the presets assume Homebrew's Qt. On Linux
install `qt6-base-dev` (6.6+), `cmake`, `ninja-build` and a C++20 compiler,
then `cmake --preset debug` works as above (the missing Homebrew path is
ignored). On Windows run from a Visual Studio developer prompt with
`QT_PREFIX` pointing at e.g. `C:\Qt\6.8.3\msvc2022_64`.

## Continuous integration and releases

`.github/workflows/build.yml` builds the `ci` preset on macOS, Linux and
Windows runners for pull requests and pushes to any branch except `master`,
and smoke-starts the Linux binary with the offscreen platform plugin.

`.github/workflows/release.yml` runs on every push to `master`: it builds
the same preset on all three platforms, packages the result and publishes
a GitHub release tagged `v<version>-<run number>` with

- `bsc-macos.dmg` (`packaging/macos.sh`: macdeployqt, pruned, ad-hoc signed),
- `bsc-linux-x86_64.AppImage` (`packaging/linux.sh`: linuxdeploy + Qt plugin,
  built on Ubuntu 22.04 so it runs on anything newer),
- `bsc-windows-x64.zip` (windeployqt output, no installer).

The packages are not code-signed with a developer certificate. Both
packaging scripts can be run locally after a release build, e.g.
`QT_ROOT_DIR=$(brew --prefix qt) packaging/macos.sh build/release/bsc.app bsc.dmg`.

## Universal binary / distribution

A universal binary (Apple Silicon + Intel) needs a Qt built for both
architectures (e.g. from the Qt online installer):

```sh
cmake --preset release -DCMAKE_OSX_ARCHITECTURES="arm64;x86_64"
```

Self-contained bundle: `macdeployqt build/release/bsc.app -dmg`.
