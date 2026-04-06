#!/usr/bin/env bash
set -e

ROOT_DIR="$(pwd)"
BUILD_DIR="${ROOT_DIR}/build"

if [ ! -d "${BUILD_DIR}" ] || [ ! -f "${BUILD_DIR}/Makefile" ]; then
  cmake -S "${ROOT_DIR}" -B "${BUILD_DIR}"
fi

if [ $# -eq 0 ]; then
  (
    cd "${BUILD_DIR}"
    cmake --build . --target help \
      | sed -n 's/^... \([a-zA-Z0-9_][^ ]*\)$/\1/p' \
      | grep -v -E '^(all|clean|depend|edit_cache|rebuild_cache)$' \
      | sort
  )
  exit 0
fi

ARG="$1"

if [[ "${ARG}" == *.cpp ]]; then
  BASENAME="$(basename "${ARG}")"
  TARGET="${BASENAME%.cpp}"
else
  TARGET="${ARG}"
fi

(
  cd "${BUILD_DIR}"
  cmake --build . --target "${TARGET}"
)

BIN="${BUILD_DIR}/${TARGET}"
if [ -x "${BIN}" ]; then
  "${BIN}"
else
  exit 1
fi

