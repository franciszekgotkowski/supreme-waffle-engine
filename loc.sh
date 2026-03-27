#!/bin/bash

# Define the directories to search
TARGET_DIRS=(
  "engine/src"
  "engine/headers/engine"
  "playground/src"
  "playground/headers/playground"
  "common/src"
  "common/headers/common"
  "platform/src"
  "platform/shaders"
  "platform/headers/export"
  "platform/headers/internal"
)

# Use find to locate .c and .h files and pass them to wc -l
find "${TARGET_DIRS[@]}" -type f \( -name "*.c" -o -name "*.h" -o -name "*.vert" -o -name "*.frag" \) -exec wc -l {} +
