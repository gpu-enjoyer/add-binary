#! /bin/bash

THIS_DIR="$(dirname "$(realpath "$0")")"
source "$THIS_DIR"/.env

"$THIS_DIR"/build.sh || (echo -e "$(cyan BUILD_ERROR)" && exit 1)
"$THIS_DIR"/run.sh   || (echo -e "$(cyan RUN_ERROR)"   && exit 1)

exit 0
