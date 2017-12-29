#!/bin/bash

IWYU_WARN_LOG=$(mktemp -t iwyu_warn.XXXXXX)
IWYU_ALL_LOG=$(mktemp -t iwyu_all.XXXXXX)
trap "rm -f $IWYU_WARN_LOG $IWYU_ALL_LOG" EXIT

python -u /usr/local/bin/iwyu_tool.py --j=$(nproc) -p . -- --mapping_file=../tools/iwyu/project.imp --check_also=../src/utils/* 2>&1 | tee $IWYU_ALL_LOG

echo "IWYU: checking output"
cat $IWYU_ALL_LOG | awk -f ../tools/iwyu/iwyu.awk | tee $IWYU_WARN_LOG

if [ -s "$IWYU_WARN_LOG" ]; then
  echo "IWYU: Warings were generated"
  exit 1
fi

echo "IWYU: no warnings"
exit 0
