#!/bin/bash
# Turns a built bsc.app into a self-contained, ad-hoc signed DMG.
# Usage: packaging/macos.sh <path/to/bsc.app> <output.dmg>
# Needs QT_ROOT_DIR (Qt prefix, e.g. $(brew --prefix qt)).
set -euo pipefail
APP=$1
OUT=$2
QT_BIN="${QT_ROOT_DIR:?set QT_ROOT_DIR to the Qt prefix}/bin"
FRAMEWORKS="$APP/Contents/Frameworks"
PLUGINS="$APP/Contents/PlugIns"

# Homebrew's Qt is split into many kegs whose libraries are referenced by
# @rpath; -libpath lets macdeployqt resolve them. It still complains about
# frameworks it cannot find for plugins we remove below, so its exit code
# is not trusted; the checks at the end are.
BREW=$(brew --prefix 2>/dev/null || echo /opt/homebrew)
"$QT_BIN/macdeployqt" "$APP" -libpath="$BREW/lib" -libpath="$BREW/opt/qtsvg/lib" || true

# macdeployqt resolves a plugin's @rpath only through the plugin's own rpath,
# which points at the qtbase keg, so frameworks from other kegs that only
# plugins need are copied by hand (dyld finds them via the executable's
# rpath, @executable_path/../Frameworks).
for keg_framework in "$BREW/opt/qtsvg/lib/QtSvg.framework"; do
   name=$(basename "$keg_framework")
   if [ ! -d "$FRAMEWORKS/$name" ]; then
      cp -R "$keg_framework" "$FRAMEWORKS/"
      rm -rf "$FRAMEWORKS/$name/Headers" "$FRAMEWORKS/$name/Versions/A/Headers"
      chmod -R u+w "$FRAMEWORKS/$name"
      bin="$FRAMEWORKS/$name/Versions/A/${name%.framework}"
      # Point its id and its Qt dependencies at the bundle instead of the kegs.
      install_name_tool -id "@rpath/$name/Versions/A/${name%.framework}" "$bin"
      for dep in $(otool -L "$bin" | awk 'NR>1 && $1 ~ /^\/opt\/homebrew\/.*\.framework\// {print $1}'); do
         install_name_tool -change "$dep" "@rpath/${dep#*/lib/}" "$bin"
      done
   fi
done

# Plugins the app does not use; they pull in Qt Quick, Qt Qml and Qt Pdf.
rm -f "$PLUGINS/platforminputcontexts/libqtvirtualkeyboardplugin.dylib" \
      "$PLUGINS/imageformats/libqpdf.dylib"

# Keep only the frameworks/dylibs reachable from the executable and plugins.
# Entry in Contents/Frameworks (X.framework or libX.dylib) for each @rpath /
# @executable_path / @loader_path reference of a Mach-O file.
refs() { otool -L "$1" | awk 'NR>1 && $1 ~ /^@/ {print $1}' | sed -E -e 's#.*/([^/]+\.framework)/.*#\1#' -e 't' -e 's#.*/##'; }
binary_of() { case "$1" in *.framework) echo "$FRAMEWORKS/$1/Versions/A/${1%.framework}";; *) echo "$FRAMEWORKS/$1";; esac; }
keep=$( { refs "$APP/Contents/MacOS/bsc"; for p in $(find "$PLUGINS" -name '*.dylib'); do refs "$p"; done; } | sort -u )
while :; do
   more=$( for k in $keep; do b=$(binary_of "$k"); [ -f "$b" ] && refs "$b"; done | sort -u )
   merged=$( printf '%s\n%s\n' "$keep" "$more" | sort -u )
   [ "$merged" = "$keep" ] && break
   keep=$merged
done
for entry in "$FRAMEWORKS"/*; do
   name=$(basename "$entry")
   if ! grep -qx "$name" <<<"$keep"; then
      echo "pruning unused $name"
      rm -rf "$entry"
   fi
done

# Every reference must resolve inside the bundle.
for f in "$APP/Contents/MacOS/bsc" $(find "$PLUGINS" "$FRAMEWORKS" -type f \( -name '*.dylib' -o -perm +111 \)); do
   for r in $(refs "$f"); do
      [ -f "$(binary_of "$r")" ] || { echo "ERROR: $f references missing $r" >&2; exit 1; }
   done
done

# Nothing may still point outside the bundle.
leaks=$( find "$APP/Contents" -type f \( -name '*.dylib' -o -perm +111 \) -exec otool -l {} + 2>/dev/null | grep -A2 LC_LOAD_DYLIB | grep -c -E '/opt/homebrew|/usr/local/' || true )
if [ "$leaks" != "0" ]; then
   echo "ERROR: $leaks load command(s) still reference Homebrew paths" >&2
   exit 1
fi

codesign --force --deep --sign - "$APP"
codesign --verify --deep --strict "$APP"

rm -f "$OUT"
hdiutil create -quiet -volname "Beesoft Commander" -srcfolder "$APP" -ov -format UDZO "$OUT"
echo "wrote $OUT ($(du -h "$OUT" | cut -f1))"
