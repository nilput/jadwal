SCRIPTDIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
PROJECTDIR="$SCRIPTDIR/../"
pushd "$PROJECTDIR"
make -C bench $*
make -f bench/mkbenchstdunordered.mk $*
make -f bench/mkbenchsparsehash.mk $*
popd
