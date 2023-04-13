#!/usr/bin/env bash

set -e
realpath() {
  OURPWD=$PWD
  cd "$(dirname "$1")"
  LINK=$(readlink "$(basename "$1")")
  while [ "$LINK" ]; do
    cd "$(dirname "$LINK")"
    LINK=$(readlink "$(basename "$1")")
  done
  REALPATH="$PWD/$(basename "$1")"
  cd "$OURPWD"
  echo "$REALPATH"
}
SCRIPT_NAME=
if [ -z $BASH_SOURCE ]; then
  SCRIPT_NAME=$0
else
  SCRIPT_NAME=${BASH_SOURCE[0]}
fi
SCRIPT_DIR=$(realpath "$(dirname "$SCRIPT_NAME")")
source $SCRIPT_DIR/env.sh

CF=clang-format
# FIXME: required until LLVM 16 when fix for .inc hits the release
CF=$SAPPHIRE_HOME/../llvm/llvm-project/build/bin/clang-format

DIRS=("src" "runtime")
FILES=("*.c" "*.cpp" "*.cc" "*.h" "*.hpp" "*.inc")
for d in "${DIRS[@]}"; do
  for s in "${FILES[@]}"; do
    (set -x && find $SAPPHIRE_HOME/$d -ipath "$s" -type f -exec $CF --Werror --style=file:$SAPPHIRE_HOME/.clang-format -i {} +)
  done
done

DIRS=("src" "scripts")
for d in "${DIRS[@]}"; do
  (set -x && find $SAPPHIRE_HOME/$d -ipath '*.py' -type f -exec python3 -m black --quiet {} +)
done
