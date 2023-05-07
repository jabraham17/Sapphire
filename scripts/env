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


export SAPPHIRE_HOME=$SCRIPT_DIR/..
export SAPPHIRE_SCRIPTS=$SAPPHIRE_HOME/scripts

export SAPPHIRE_TEST=$SAPPHIRE_HOME/test
export SAPPHIRE_TEST_BUILD=$SAPPHIRE_TEST/build
