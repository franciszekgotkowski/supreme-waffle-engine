#!/bin/bash

# Define the directories to search
TARGET_DIRS=(
  "engine/src"
  "engine/headers/engine"
  "game/src"
  "game/headers/game"
)

# Use find to locate .c and .h files and pass them to wc -l
find "${TARGET_DIRS[@]}" -type f \( -name "*.c" -o -name "*.h" -o -name "*.vert" -o -name "*.frag" \) -exec wc -l {} +