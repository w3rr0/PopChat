#!/bin/bash

source .env

# Reload configuration (detect new file)
echo "--- CMake config ---"
cmake -B build -DCMAKE_PREFIX_PATH=$DCMAKE_PREFIX_PATH -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

# Compile
echo "--- Building project ---"
if cmake --build build; then
    echo "--- App logs ---"
    # Run - use & to prevent terminal freez
    ./build/PopChat.app/Contents/MacOS/PopChat
else
    echo "[ERROR]: Compilation failed"
fi
