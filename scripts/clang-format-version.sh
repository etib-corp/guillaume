# Sourced by scripts/run-clang-format.sh and .githooks/pre-commit.
#
# Finds clang-format and checks that it is the pinned major version, so that
# a developer and CI produce the same formatting. Sets CLANG_FORMAT_BIN.
#
# Environment:
#   CLANG_FORMAT                         Binary to use (name or path).
#                                        Default: clang-format-<major>, then
#                                        clang-format.
#   CLANG_FORMAT_ALLOW_VERSION_MISMATCH  Set to 1 to run with another major
#                                        version. CI will not accept the
#                                        result if it formats differently.

# The one place the version is pinned. The CI format job must install the
# same major version (.github/workflows/format.yml).
REQUIRED_CLANG_FORMAT_MAJOR=21

resolve_clang_format() {
	local candidate version major

	CLANG_FORMAT_BIN=""
	if [ -n "${CLANG_FORMAT:-}" ]; then
		if ! command -v "$CLANG_FORMAT" > /dev/null 2>&1; then
			echo "Error: CLANG_FORMAT is set to '$CLANG_FORMAT'," \
				"which is not an executable." >&2
			return 1
		fi
		CLANG_FORMAT_BIN="$CLANG_FORMAT"
	else
		for candidate in \
			"clang-format-$REQUIRED_CLANG_FORMAT_MAJOR" clang-format; do
			if command -v "$candidate" > /dev/null 2>&1; then
				CLANG_FORMAT_BIN="$candidate"
				break
			fi
		done
	fi

	if [ -z "$CLANG_FORMAT_BIN" ]; then
		cat >&2 << EOF
Error: clang-format was not found on PATH.

This repository is formatted with clang-format $REQUIRED_CLANG_FORMAT_MAJOR.x. Install it, for example:
  Debian/Ubuntu: sudo apt-get install clang-format-$REQUIRED_CLANG_FORMAT_MAJOR
  macOS:         brew install llvm@$REQUIRED_CLANG_FORMAT_MAJOR
                 (or the Xcode Command Line Tools, when they ship version $REQUIRED_CLANG_FORMAT_MAJOR)
  pip:           pip install 'clang-format==$REQUIRED_CLANG_FORMAT_MAJOR.*'

If it is installed outside PATH, point CLANG_FORMAT at it:
  CLANG_FORMAT=/path/to/clang-format $0
EOF
		return 1
	fi

	version="$("$CLANG_FORMAT_BIN" --version 2> /dev/null || true)"
	major="$(printf '%s\n' "$version" |
		sed -n 's/.*version \([0-9][0-9]*\)\..*/\1/p' | head -n 1)"

	if [ "$major" = "$REQUIRED_CLANG_FORMAT_MAJOR" ]; then
		return 0
	fi

	if [ "${CLANG_FORMAT_ALLOW_VERSION_MISMATCH:-0}" = "1" ]; then
		echo "Warning: using '$CLANG_FORMAT_BIN' (${version:-unknown version})" \
			"instead of clang-format $REQUIRED_CLANG_FORMAT_MAJOR.x;" \
			"CI may format differently." >&2
		return 0
	fi

	cat >&2 << EOF
Error: clang-format $REQUIRED_CLANG_FORMAT_MAJOR.x is required, but '$CLANG_FORMAT_BIN' reports:
  ${version:-(no version could be read)}

Another major version formats some constructs differently, and CI checks the
result with version $REQUIRED_CLANG_FORMAT_MAJOR. Either:
  - install clang-format $REQUIRED_CLANG_FORMAT_MAJOR and, if it is not the default, set
    CLANG_FORMAT=/path/to/clang-format-$REQUIRED_CLANG_FORMAT_MAJOR
  - or set CLANG_FORMAT_ALLOW_VERSION_MISMATCH=1 to run anyway.
EOF
	return 1
}
