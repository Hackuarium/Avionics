#!/bin/bash

# Base directory (change '.' if needed)
BASE_DIR="."

# Step 1: Rename files and directories
find "$BASE_DIR" -depth -name "*5outputs*" | while IFS= read -r path; do
    new_path=$(echo "$path" | sed 's/5outputs/meteo/g')
    if [ "$path" != "$new_path" ]; then
        mv "$path" "$new_path"
        echo "Renamed: $path -> $new_path"
    fi
done
