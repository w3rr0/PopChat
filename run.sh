#!/bin/bash

# Reload configuration (detect new file)
echo "--- CMake config ---"
cmake -B build -DCMAKE_PREFIX_PATH=/Users/mac/Qt/6.10.1/macos

# Compile
echo "--- Building project ---"
if cmake --build build; then
    echo "--- Run app ---"
    # Run - use & to prevent terminal freez
    ./build/PopChat.app/Contents/MacOS/PopChat &
else
    echo "[ERROR]: Compilation failed"
fi
