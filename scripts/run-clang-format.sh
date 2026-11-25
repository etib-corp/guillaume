#!/bin/bash
set -euo pipefail

# Formats every C++ file of the repository in place: *.cpp, *.hpp and *.tpp.
#
# Skipped: hidden directories (.git, .claude/worktrees, ...), build/,
# cmake_build/ and third_party/.
#
# The clang-format major version is pinned in scripts/clang-format-version.sh;
# see that file for CLANG_FORMAT and CLANG_FORMAT_ALLOW_VERSION_MISMATCH.

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
ROOT_DIR="$( cd "$SCRIPT_DIR/.." && pwd )"

# shellcheck source=scripts/clang-format-version.sh
source "$SCRIPT_DIR/clang-format-version.sh"
resolve_clang_format || exit 1

echo "Formatting C++ files in $ROOT_DIR..."

# Paths are relative to the root, so that a checkout which itself lives under
# a hidden directory (a Git worktree, for example) is still formatted.
cd "$ROOT_DIR"

find . \
    -type d \( \
        -name ".?*" \
        -o -name "build" \
        -o -name "cmake_build" \
        -o -name "third_party" \
    \) -prune \
    -o -type f \( -name "*.cpp" -o -name "*.hpp" -o -name "*.tpp" \) \
    -print0 |
xargs -0 "$CLANG_FORMAT_BIN" -i

echo "All C++ files have been formatted."
