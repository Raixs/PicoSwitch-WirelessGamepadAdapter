#!/bin/bash

# Script to apply BTStack compatibility patch
# Usage: ./apply-btstack-patch.sh

PATCH_FILE="patches/bluepad32-btstack-fix.patch"
TARGET_FILE="bluepad32/src/components/bluepad32/parser/uni_hid_parser.c"

echo "Applying BTStack compatibility patch..."

if [ ! -f "$PATCH_FILE" ]; then
    echo "Error: Patch file not found: $PATCH_FILE"
    exit 1
fi

if [ ! -f "$TARGET_FILE" ]; then
    echo "Error: Target file not found: $TARGET_FILE"
    echo "Make sure you have initialized submodules with: make update"
    exit 1
fi

# Check if patch is already applied
if grep -q "parser.usage_iterator.global_logical_minimum" "$TARGET_FILE"; then
    echo "Patch already applied!"
    exit 0
fi

# Apply the patch
if patch -p1 < "$PATCH_FILE"; then
    echo "Patch applied successfully!"
    echo "You can now build the project with: make build"
else
    echo "Failed to apply patch!"
    echo "You may need to apply the changes manually. See README.md for details."
    exit 1
fi
