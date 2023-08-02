#!/bin/bash

cd DB

for dir in "$@"; do

    if [ -d "$dir" ]; then
        echo "Removing directory: $dir"
        rm -r "$dir"
    else
        echo "Directory not found: $dir"
    fi

done

cd ..