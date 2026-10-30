#!/bin/sh
set -eu

STB_DIR="includes/stb"
STB_VERSION="2.30"

STB_FILE="$STB_DIR/stb_image.h"

if [ -f "$STB_FILE" ]; then
  echo "stb_image is already installed."
  exit 0
fi

mkdir -p "$STB_DIR"

curl -fL \
  "https://raw.githubusercontent.com/nothings/stb/master/stb_image.h" \
  -o "$STB_FILE"

echo "stb_image installation completed."
