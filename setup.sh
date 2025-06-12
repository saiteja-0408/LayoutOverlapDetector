#!/usr/bin/env bash
set -euo pipefail
echo "Detecting OS..."
case "$OSTYPE" in
  linux*) OS=linux;;
  darwin*) OS=macos;;
  *) echo "Unsupported OS"; exit 1;;
esac

echo "Installing prerequisites..."
if [ "$OS" = "linux" ]; then
  sudo apt-get update
  sudo apt-get install -y build-essential cmake qt6-base-dev qt6-declarative-dev                python3 python3-pip libnlohmann-json3-dev libgtest-dev
  if [ -d /usr/src/gtest ]; then
    sudo cmake -S /usr/src/gtest -B /usr/src/gtest/build -DBUILD_GMOCK=OFF
    sudo cmake --build /usr/src/gtest/build --target install
  fi
else
  brew update
  brew install cmake qt6 nlohmann-json googletest python@3 || true
  brew link --force googletest || true
fi

echo "Generating sample layout..."
scripts/generate_layout.py

echo "Configuring build..."
mkdir -p build && cd build
if [ "$OS" = "macos" ]; then
  cmake .. -DCMAKE_PREFIX_PATH="$(brew --prefix qt6)/lib/cmake/Qt6" -DCMAKE_BUILD_TYPE=Release
else
  cmake .. -DCMAKE_BUILD_TYPE=Release
fi

echo "Building..."
cmake --build .

echo "Launching application..."
./LayoutOverlapDetector
