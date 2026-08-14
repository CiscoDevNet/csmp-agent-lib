#!/bin/sh

# -------------------------------------------------------------------------------------
# Wireshark CoAP/CSMP Dissector installer for Linux/Mac
#
# Usage : ./install.sh [--help] [--target DIRECTORY] [--dry-run]
# Author: Manojna CSL <mcsl@cisco.com>, <manojnacsl@gmail.com>
# -------------------------------------------------------------------------------------

set -eu

SCRIPT_DIR=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
CSMP_LUA_FILE="coap_csmp_dissector.lua"
CSMP_PROTO_FILE="csmp.proto"
TARGET_DIR=""
DRY_RUN=0
TEMP_FILE=""

usage()
{
    cat <<EOF
Usage: ./install.sh [--target DIRECTORY] [--dry-run]

Install $CSMP_LUA_FILE and $CSMP_PROTO_FILE into Wireshark's personal Lua plugin
directory on macOS, Linux, or Windows with Git Bash/MSYS2. By default, the
directory is discovered using \`tshark -G folders\`.

Options:
  --target DIRECTORY  Install into DIRECTORY instead of auto-detecting it.
  --dry-run           Show what would be installed without changing files.
  -h, --help          Show this help text.
EOF
}

fail()
{
    printf 'Error: %s\n' "$*" >&2
    exit 1
}

cleanup()
{
    if [ -n "$TEMP_FILE" ] && [ -e "$TEMP_FILE" ]; then
        rm -f "$TEMP_FILE"
    fi
}

trap cleanup EXIT HUP INT TERM

while [ "$#" -gt 0 ]; do
    case "$1" in
        --target)
            [ "$#" -ge 2 ] || fail "--target requires a directory"
            TARGET_DIR=$2
            shift 2
            ;;
        --dry-run)
            DRY_RUN=1
            shift
            ;;
        -h|--help)
            usage
            exit 0
            ;;
        *)
            fail "unknown option: $1"
            ;;
    esac
done

find_tshark()
{
    if command -v tshark >/dev/null 2>&1; then
        command -v tshark
        return
    fi

    # Default Wireshark application location on macOS.
    if [ -x /Applications/Wireshark.app/Contents/MacOS/tshark ]; then
        printf '%s\n' /Applications/Wireshark.app/Contents/MacOS/tshark
        return
    fi

    # Default Wireshark locations when running under Git Bash/MSYS2.
    for candidate in \
        '/c/Program Files/Wireshark/tshark.exe' \
        '/c/Program Files (x86)/Wireshark/tshark.exe'
    do
        if [ -x "$candidate" ]; then
            printf '%s\n' "$candidate"
            return
        fi
    done

    return 1
}

detect_target_directory()
{
    tshark_path=$(find_tshark) || return 1

    detected_directory=$(
        "$tshark_path" -G folders 2>/dev/null |
            sed -n 's/^Personal Lua Plugins:[[:space:]]*//p' |
            sed -n '1p' |
            tr -d '\r'
    )

    [ -n "$detected_directory" ] || return 1

    # Convert a Windows path returned by tshark.exe for Git Bash/MSYS2.
    if command -v cygpath >/dev/null 2>&1; then
        converted_directory=$(cygpath -u "$detected_directory" 2>/dev/null) ||
            converted_directory=$detected_directory
        detected_directory=$converted_directory
    fi

    printf '%s\n' "$detected_directory"
}

fallback_target_directory()
{
    platform=$(uname -s 2>/dev/null || printf 'unknown')

    case "$platform" in
        CYGWIN*|MINGW*|MSYS*)
            [ -n "${APPDATA:-}" ] || return 1
            windows_directory="$APPDATA\\Wireshark\\plugins"

            if command -v cygpath >/dev/null 2>&1; then
                cygpath -u "$windows_directory"
            else
                printf '%s\n' "$windows_directory"
            fi
            ;;
        Darwin*|Linux*)
            [ -n "${HOME:-}" ] || return 1
            printf '%s\n' "$HOME/.local/lib/wireshark/plugins"
            ;;
        *)
            return 1
            ;;
    esac
}

if [ -z "$TARGET_DIR" ]; then
    TARGET_DIR=$(detect_target_directory) ||
        TARGET_DIR=$(fallback_target_directory) || fail \
            "unable to locate the plugin directory; use --target"

    [ -n "$TARGET_DIR" ] || fail \
        "tshark did not report a Personal Lua Plugins directory"
fi

for required_file in "$CSMP_LUA_FILE" "$CSMP_PROTO_FILE"; do
    [ -f "$SCRIPT_DIR/$required_file" ] || fail \
        "required source file is missing: $SCRIPT_DIR/$required_file"
done

printf 'Wireshark personal Lua plugin directory: %s\n' "$TARGET_DIR"

if [ "$DRY_RUN" -eq 0 ]; then
    mkdir -p "$TARGET_DIR"
fi

install_file()
{
    source_file=$1
    destination_file=$2

    if [ -e "$destination_file" ] &&
        [ "$source_file" -ef "$destination_file" ]; then
        printf 'Already installed: %s\n' "$destination_file"
        return
    fi

    if [ -f "$destination_file" ] &&
        cmp -s "$source_file" "$destination_file"; then
        printf 'Up to date: %s\n' "$destination_file"
        return
    fi

    if [ "$DRY_RUN" -eq 1 ]; then
        printf 'Would install: %s -> %s\n' \
            "$source_file" "$destination_file"
        return
    fi

    TEMP_FILE="$TARGET_DIR/.$(basename "$destination_file").tmp.$$"
    cp "$source_file" "$TEMP_FILE"
    chmod 0644 "$TEMP_FILE"
    mv -f "$TEMP_FILE" "$destination_file"
    TEMP_FILE=""

    cmp -s "$source_file" "$destination_file" || fail \
        "verification failed for $destination_file"

    printf 'Installed: %s\n' "$destination_file"
}

install_file \
    "$SCRIPT_DIR/$CSMP_LUA_FILE" \
    "$TARGET_DIR/$CSMP_LUA_FILE"

install_file \
    "$SCRIPT_DIR/$CSMP_PROTO_FILE" \
    "$TARGET_DIR/$CSMP_PROTO_FILE"

if [ "$DRY_RUN" -eq 1 ]; then
    printf 'Dry run complete; no files were changed.\n'
else
    printf '%s\n' \
        'Installation complete. Restart Wireshark or reload Lua plugins.'
fi
