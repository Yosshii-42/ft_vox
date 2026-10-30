#!/bin/sh
set -eu

GLAD_DIR="includes/glad"
GLAD_VERSION="0.1.36"

if [ -f "$GLAD_DIR/include/glad/glad.h" ] &&
   [ -f "$GLAD_DIR/include/KHR/khrplatform.h" ] &&
   [ -f "$GLAD_DIR/src/glad.c" ]; then
   echo "GLAD is already installed."
   exit 0
fi

# GLAD生成ツールをtmp directoryにインストール
TEMP_DIR=$(mktemp -d)
trap 'rm -rf "$TEMP_DIR"' EXIT

python3 -m pip install \
  --target "$TEMP_DIR" \
  "glad==$GLAD_VERSION"

# OpenGL 3.3 Core用のGLADを生成
PYTHONPATH="$TEMP_DIR" python3 -m glad \
  --profile core \
  --api "gl=3.3" \
  --generator c \
  --spec gl \
  --extensions "" \
  --out-path "$GLAD_DIR"
# profile core: Core Profileを指定する
# api "gl=3.3": OpenGLのバージョン3.3を指定する
# generator c: c言語用に生成する
# spec gl: SpecificationをOpenGLに指定する

echo "GLAD installation completed."

