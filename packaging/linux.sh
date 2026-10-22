#!/bin/bash
# Builds an AppImage from an installed tree.
# Usage: packaging/linux.sh <build-dir> <output.AppImage>
# Needs QT_ROOT_DIR (Qt prefix) and a Qt-built bsc in <build-dir>.
set -euo pipefail
BUILD=$1
# Absolute, because the script changes directory below.
OUT=$(mkdir -p "$(dirname "$2")" && cd "$(dirname "$2")" && pwd)/$(basename "$2")
SRC=$(cd "$(dirname "$0")/.." && pwd)
WORK=$(mktemp -d)

cmake --install "$BUILD" --prefix "$WORK/AppDir/usr"

cd "$WORK"
curl -sSLo linuxdeploy.AppImage \
   https://github.com/linuxdeploy/linuxdeploy/releases/download/continuous/linuxdeploy-x86_64.AppImage
# linuxdeploy finds the plugin next to itself by this exact file name.
curl -sSLo linuxdeploy-plugin-qt-x86_64.AppImage \
   https://github.com/linuxdeploy/linuxdeploy-plugin-qt/releases/download/continuous/linuxdeploy-plugin-qt-x86_64.AppImage
chmod +x linuxdeploy.AppImage linuxdeploy-plugin-qt-x86_64.AppImage

cat > bsc.desktop <<DESKTOP
[Desktop Entry]
Type=Application
Name=Beesoft Commander
Comment=Two-panel file manager
Exec=bsc
Icon=bsc
Categories=Utility;FileManager;
DESKTOP

export APPIMAGE_EXTRACT_AND_RUN=1          # no FUSE needed to run the tools
export QMAKE="${QT_ROOT_DIR:?set QT_ROOT_DIR to the Qt prefix}/bin/qmake"
export OUTPUT="$WORK/bsc.AppImage"
./linuxdeploy.AppImage --appdir AppDir -e AppDir/usr/bin/bsc -d bsc.desktop -i "$SRC/img/bsc.png" \
   --plugin qt --output appimage

mv "$OUTPUT" "$OUT"
rm -rf "$WORK"
echo "wrote $OUT ($(du -h "$OUT" | cut -f1))"
