#!/usr/bin/env bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
SOURCE_DIR=$DIR/../

function BuildWidelands() {
   PREFIX_PATH=$(brew --prefix)
   eval "$($PREFIX_PATH/bin/brew shellenv)"
   export CMAKE_PREFIX_PATH="${PREFIX_PATH}/opt/icu4c"

   echo "FIXED ICU Issue $CMAKE_PREFIX_PATH"

   pushd $SOURCE_DIR
   ./compile.sh --release --clang
   popd

   echo "Done building."
}

BuildWidelands
