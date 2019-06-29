#!/bin/bash
SCRIPTDIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
INPUT_HEADER="$SCRIPTDIR/../src/hasht.h"
gen() {
    prefix="$1";
    output="$2";
    sed "s/hasht/${prefix}/g ; s/HASHT/${prefix^^}/g" "$INPUT_HEADER"  > "$output" || exit 1;
}
if [ $# -lt 2 ]; then
    echo "error, script usage:\
./gen_hasht [prefix] [output_header_name]" >&2;
fi
gen "$1" "$2";
