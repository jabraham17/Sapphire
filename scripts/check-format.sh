#!/usr/bin/env bash

# set -e
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

export CF=clang-format
# FIXME: required until LLVM 16 when fix for .inc hits the release
export CF=$SAPPHIRE_HOME/../llvm/llvm-project/build/bin/clang-format
export STYLE=$SAPPHIRE_HOME/.clang-format

check_format() {
  file=$1
  diff <($CF --Werror --style=file:$STYLE $file) $file >/dev/null 2>&1
}
check_format_python() {
  file=$1
  python3 -m black --quiet --check $file >/dev/null 2>&1
}
export -f check_format
export -f check_format_python

DIRS=("src" "runtime")
FILES=("*.c" "*.cpp" "*.cc" "*.h" "*.hpp" "*.inc")
for d in "${DIRS[@]}"; do
  for s in "${FILES[@]}"; do
    (find $SAPPHIRE_HOME/$d -ipath "$s" -type f -exec bash -c 'check_format "$0" && test $? -eq 0 || echo $(echo $0 | sed "s;$SAPPHIRE_HOME/;;g") needs to be formatted' {} \; )
  done
done

DIRS=("src" "scripts")
for d in "${DIRS[@]}"; do
  (find $SAPPHIRE_HOME/$d -ipath '*.py' -type f -exec bash -c 'check_format_python "$0" && test $? -eq 0 || echo $(echo $0 | sed "s;$SAPPHIRE_HOME/;;g") needs to be formatted' {} \; )
done
