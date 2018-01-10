#!/bin/bash
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd $DIR/..

bps=$(find . -name "*.bp")
bpfmt -w $bps
git diff --exit-code HEAD $bps
exit $?
