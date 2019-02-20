#!/bin/sh
#
# Make samples for OS X
#

NSC="../../bin/nsc -n"

FILES=(\
sw/Mozart_Kv618.mml \
sw/Bach_BWV578.MML)


LEN=${#FILES[@]}
LEN=$((LEN - 1))

for idx in $(seq 0 $LEN)
do
  pushd .
  path=${FILES[$idx]}
  file=$(basename $path)
  dir=$(dirname $path)

  cd $dir
  echo "Compiling $file..."
  $NSC $file
  popd
done

