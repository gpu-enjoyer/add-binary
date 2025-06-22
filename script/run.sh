#!/bin/bash

THIS_DIR="$(dirname "$0")"
source "$THIS_DIR"/.env

cd "$THIS_DIR"/../build || exit 1

    ./add-binary

cd "$ORIG_DIR" || exit 1

echo -e "$MSG done" && exit 0
