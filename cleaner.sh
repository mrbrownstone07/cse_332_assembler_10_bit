#!/bin/bash

cd /home/m/Code/assembler/

# Exit if the directory isn't found.
if (($?>0)); then
    echo "Can't find work dir... exiting"
    exit
fi

for i in *; do
    if ! grep -qxFe "$i" filelist.txt; then
        echo ">> Enter 0 to delete: $i"
        read arg
        
        if (($arg == 0 )); then
        echo ">> Deleted: $i"
        rm "$i"
        fi
    fi
done

