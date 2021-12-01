#!/bin/sh
set -eu

# Find the source directory
cwd="$(pwd)"
cd "$(dirname "$0")"
srcdir="$(pwd)"
cd "${cwd}"

# Parse command line options
if [ $# -ne 3 ]; then
    echo "Usage: $0 <day> <puzzle> <input-file>" >&2
    exit 1
fi

day="$1"
num="$2"
input="$3"

if [ ${#day} -eq 1 ]; then
    day="0${day}"
fi

if [ ${#num} -eq 1 ]; then
    num="0${num}"
fi

puzzle="puzzle-${day}-${num}"
exe="build/${puzzle}/${puzzle}"

if [ ! -d "${srcdir}/${puzzle}" ]; then
    echo "Unable to find puzzle sources for day ${day} number ${num}" >&2
fi

if [ ! -e "${exe}" ]; then
    opts=
    if [ "$(uname -s)" == "Darwin" ]; then
      if [ "$(uname -m)" == "arm64" ]; then
        opts="-DOPENSSL_ROOT_DIR=/opt/homebrew/opt/openssl"
      else
        opts="-DOPENSSL_ROOT_DIR=/usr/local/opt/openssl"
      fi
    fi
    cmake -Bbuild -S "${srcdir}" ${opts}
    cmake --build build --target "${puzzle}"
fi

if [ -e "${srcdir}/${puzzle}/driver.sh" ]; then
    "${srcdir}/${puzzle}/driver.sh" "${exe}" "${input}"
else
    "${exe}" <"${input}"
fi
