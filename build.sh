#!/usr/bin/env bash
set -euo pipefail


# 1) Clean up previous configuration and cache
[ -f Makefile ] && make distclean || true
rm -f config.cache

# 2) Run autogen and configure
./autogen.sh
echo "[STEP] autogen.sh done"
sleep 1

PREFIX="/home/arkade/github/memory_allocator/compare_allocators__glibc_vs_binning/allocators/TCMalloc__binning_allocator/tcmalloc_dev"
./configure --prefix="$PREFIX"
echo "[STEP] configure done (prefix=$PREFIX)"
sleep 1

# 3) Build with verbose output to capture compile commands
#    Store the full log to check if flags were applied
make -j"$(nproc)" V=1 |& tee build.log
echo "[STEP] make done"
sleep 1

# 4) Install
make install
echo "[DONE] installed to $PREFIX"
