#!/bin/bash

set -euo pipefail

source config.sh

if [ $# -eq 0 ]; then
    echo "Usage: $0 <path>"
    echo ""
    echo "Result: all rawiv files that are below <path> in the file tree will be"
    echo "converted into inr files. Original rawivs will remain untouched"
    exit 1
fi

find "$1" -name '*.rawiv' | while read fname; do
    new_basename="$(basename -s '.rawiv' "$fname").inr"
    new_dirname="$(dirname "$fname")"
    new_fname="$new_dirname/$new_basename"
    echo "Invocation: vol2inr $fname 0 $new_fname" 
    "$VOLROVER_BIN"/vol2inr "$fname" 0 "$new_fname"
done
