#!/bin/sh

# -------------------------------------------------------------------------------------
# Add a Protobuf-backed TLV to the Wireshark CSMP dissector
#
# Usage : ./add_tlv.sh [--dry-run] TLV_ID TLV_PROTO_FILE
# Author: Manojna CSL <mcsl@cisco.com>, <manojnacsl@gmail.com>
# -------------------------------------------------------------------------------------

set -eu

SCRIPT_DIR=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
LUA_FILE="$SCRIPT_DIR/coap_csmp_dissector.lua"
PROTO_FILE="$SCRIPT_DIR/csmp.proto"
DRY_RUN=0
TEMP_DIR=""
LUA_STAGE=""
PROTO_STAGE=""
ROLLBACK_REQUIRED=0

usage()
{
    cat <<'EOF'
Usage: ./add_tlv.sh [--dry-run] TLV_ID TLV_PROTO_FILE

Add one Protobuf-backed TLV to coap_csmp_dissector.lua and csmp.proto.

Arguments:
  TLV_ID          Integer TLV identifier in the range 1..4294967295.
  TLV_PROTO_FILE  .proto file containing exactly one top-level message.

The input file may contain a proto3 syntax declaration and either the csmp
package declaration or no package declaration. Imports and multiple top-level
messages are rejected because they cannot be merged safely automatically. The
new message is inserted in ascending TLV_ID order in csmp.proto.

Options:
  --dry-run  Validate and show the proposed addition without changing files.
  -h, --help Show this help text.
EOF
}

fail()
{
    printf 'Error: %s\n' "$*" >&2
    exit 1
}

cleanup()
{
    status=$?
    trap - EXIT HUP INT TERM

    if [ "$ROLLBACK_REQUIRED" -eq 1 ] && [ -n "$TEMP_DIR" ]; then
        cp "$TEMP_DIR/csmp.lua.original" "$LUA_FILE" 2>/dev/null || true
        cp "$TEMP_DIR/csmp.proto.original" "$PROTO_FILE" 2>/dev/null || true
    fi

    [ -z "$LUA_STAGE" ] || rm -f "$LUA_STAGE"
    [ -z "$PROTO_STAGE" ] || rm -f "$PROTO_STAGE"
    [ -z "$TEMP_DIR" ] || rm -rf "$TEMP_DIR"

    exit "$status"
}

trap cleanup EXIT HUP INT TERM

if [ "${1:-}" = "--dry-run" ]; then
    DRY_RUN=1
    shift
fi

case "${1:-}" in
    -h|--help)
        usage
        exit 0
        ;;
esac

[ "$#" -eq 2 ] || {
    usage >&2
    exit 1
}

TLV_ID=$1
INPUT_PROTO=$2

case "$TLV_ID" in
    ''|*[!0-9]*)
        fail "TLV_ID must be an integer: $TLV_ID"
        ;;
esac

TLV_ID=$(awk -v id="$TLV_ID" '
    BEGIN {
        if (id < 1 || id > 4294967295) {
            exit 1
        }
        printf "%.0f", id
    }
') || fail "TLV_ID must be in the range 1..4294967295"

[ -f "$INPUT_PROTO" ] || fail "input .proto file not found: $INPUT_PROTO"
[ -f "$LUA_FILE" ] || fail "dissector file not found: $LUA_FILE"
[ -f "$PROTO_FILE" ] || fail "combined schema not found: $PROTO_FILE"

case "$INPUT_PROTO" in
    *.proto) ;;
    *) fail "input file must have a .proto extension: $INPUT_PROTO" ;;
esac

extract_top_level_messages()
{
    awk '
        function count_character(text, character, i, count) {
            count = 0
            for (i = 1; i <= length(text); i++) {
                if (substr(text, i, 1) == character) {
                    count++
                }
            }
            return count
        }

        function remove_comments(text, result, start, finish) {
            result = ""
            while (length(text) > 0) {
                if (inside_block_comment) {
                    finish = index(text, "*/")
                    if (finish == 0) {
                        return result
                    }
                    text = substr(text, finish + 2)
                    inside_block_comment = 0
                } else {
                    start = index(text, "/*")
                    finish = index(text, "//")

                    if (finish > 0 && (start == 0 || finish < start)) {
                        return result substr(text, 1, finish - 1)
                    }

                    if (start > 0) {
                        result = result substr(text, 1, start - 1)
                        text = substr(text, start + 2)
                        inside_block_comment = 1
                    } else {
                        return result text
                    }
                }
            }
            return result
        }

        {
            line = remove_comments($0)

            if (depth == 0 && line ~ /^[[:space:]]*message[[:space:]]+/) {
                name = line
                sub(/^[[:space:]]*message[[:space:]]+/, "", name)
                sub(/[[:space:]\{].*$/, "", name)
                if (name ~ /^[A-Za-z_][A-Za-z0-9_]*$/) {
                    print name
                }
            }

            depth += count_character(line, "{")
            depth -= count_character(line, "}")
            if (depth < 0) {
                exit 2
            }
        }

        END {
            if (depth != 0) {
                exit 2
            }
        }
    ' "$1"
}

TARGET_PACKAGE=$(sed -n \
    's/^[[:space:]]*package[[:space:]]\{1,\}\([A-Za-z_][A-Za-z0-9_.]*\)[[:space:]]*;.*$/\1/p' \
    "$PROTO_FILE")

[ -n "$TARGET_PACKAGE" ] || fail "cannot determine package from $PROTO_FILE"

if printf '%s\n' "$TARGET_PACKAGE" | awk 'NF { count++ } END { exit count != 1 }'; then
    :
else
    fail "$PROTO_FILE contains multiple package declarations"
fi

INPUT_PACKAGE=$(sed -n \
    's/^[[:space:]]*package[[:space:]]\{1,\}\([A-Za-z_][A-Za-z0-9_.]*\)[[:space:]]*;.*$/\1/p' \
    "$INPUT_PROTO")

if [ -n "$INPUT_PACKAGE" ]; then
    if ! printf '%s\n' "$INPUT_PACKAGE" |
        awk 'NF { count++; value = $0 } END { exit count != 1 }'; then
        fail "$INPUT_PROTO contains multiple package declarations"
    fi

    [ "$INPUT_PACKAGE" = "$TARGET_PACKAGE" ] || fail \
        "input package '$INPUT_PACKAGE' does not match '$TARGET_PACKAGE'"
fi

INPUT_SYNTAX=$(sed -n \
    's/^[[:space:]]*syntax[[:space:]]*=[[:space:]]*"\([^"]*\)"[[:space:]]*;.*$/\1/p' \
    "$INPUT_PROTO")

if [ -n "$INPUT_SYNTAX" ] && [ "$INPUT_SYNTAX" != "proto3" ]; then
    fail "input syntax must be proto3, found: $INPUT_SYNTAX"
fi

if grep -Eq '^[[:space:]]*import[[:space:]]' "$INPUT_PROTO"; then
    fail "input imports are not supported; merge dependencies first"
fi

TOP_LEVEL_MESSAGES=$(extract_top_level_messages "$INPUT_PROTO") || fail \
    "unable to parse balanced top-level message definitions in $INPUT_PROTO"

MESSAGE_COUNT=$(printf '%s\n' "$TOP_LEVEL_MESSAGES" |
    awk 'NF { count++ } END { print count + 0 }')

[ "$MESSAGE_COUNT" -eq 1 ] || fail \
    "input must contain exactly one top-level message; found $MESSAGE_COUNT"

MESSAGE_NAME=$(printf '%s\n' "$TOP_LEVEL_MESSAGES" | awk 'NF { print; exit }')
FULL_MESSAGE_NAME="$TARGET_PACKAGE.$MESSAGE_NAME"

if grep -Eq "^[[:space:]]*//[[:space:]]*TLV_ID:[[:space:]]*$TLV_ID([[:space:]]|$)" \
    "$PROTO_FILE"; then
    fail "TLV_ID $TLV_ID already exists in $PROTO_FILE"
fi

if grep -Eq "^[[:space:]]*\\[$TLV_ID\\][[:space:]]*=" "$LUA_FILE"; then
    fail "TLV_ID $TLV_ID already exists in $LUA_FILE"
fi

if extract_top_level_messages "$PROTO_FILE" | grep -Fqx "$MESSAGE_NAME"; then
    fail "message $MESSAGE_NAME already exists in $PROTO_FILE"
fi

TEMP_DIR=$(mktemp -d "${TMPDIR:-/tmp}/csmp-add-tlv.XXXXXX") || fail \
    "unable to create temporary directory"

cp "$LUA_FILE" "$TEMP_DIR/csmp.lua.original"
cp "$PROTO_FILE" "$TEMP_DIR/csmp.proto.original"

printf '// TLV_ID: %s\n' "$TLV_ID" > "$TEMP_DIR/new_tlv.proto"

awk '
    /^[[:space:]]*syntax[[:space:]]*=/ { next }
    /^[[:space:]]*package[[:space:]]+/ { next }
    /^[[:space:]]*\/\/[[:space:]]*TLV_ID:/ { next }

    skipping && /^[[:space:]]*$/ { next }
    { skipping = 0; print }
' skipping=1 "$INPUT_PROTO" >> "$TEMP_DIR/new_tlv.proto"

if ! awk -v new_id="$TLV_ID" -v new_block="$TEMP_DIR/new_tlv.proto" '
    function emit_new_block(line) {
        while ((getline line < new_block) > 0) {
            print line
        }
        close(new_block)
        print ""
        inserted = 1
    }

    /^[[:space:]]*\/\/[[:space:]]*TLV_ID:[[:space:]]*[0-9]+/ {
        existing_id = $0
        sub(/^[[:space:]]*\/\/[[:space:]]*TLV_ID:[[:space:]]*/, "", existing_id)
        sub(/[^0-9].*$/, "", existing_id)

        if (!inserted && existing_id + 0 > new_id + 0) {
            emit_new_block()
        }
    }

    { print }

    END {
        if (!inserted) {
            if (NR > 0) {
                print ""
            }
            emit_new_block()
        }
    }
' "$PROTO_FILE" > "$TEMP_DIR/csmp.proto.updated"; then
    fail "could not insert TLV $TLV_ID into $PROTO_FILE"
fi

if ! awk -v new_id="$TLV_ID" -v message="$FULL_MESSAGE_NAME" '
    function emit_entry() {
        printf "    [%d] = \"%s\",\n", new_id, message
        inserted = 1
    }

    /^local protobufMessageMap[[:space:]]*=[[:space:]]*\{/ {
        inside_map = 1
        map_found = 1
        print
        next
    }

    inside_map && /^[[:space:]]*\[[0-9]+\][[:space:]]*=/ {
        existing_id = $0
        sub(/^[[:space:]]*\[/, "", existing_id)
        sub(/\].*$/, "", existing_id)

        if (!inserted && existing_id + 0 > new_id + 0) {
            emit_entry()
        }
    }

    inside_map && /^[[:space:]]*}[[:space:]]*,?[[:space:]]*$/ {
        if (!inserted) {
            emit_entry()
        }
        inside_map = 0
    }

    { print }

    END {
        if (!map_found || !inserted) {
            exit 2
        }
    }
' "$LUA_FILE" > "$TEMP_DIR/csmp.lua.updated"; then
    fail "could not locate protobufMessageMap in $LUA_FILE"
fi

if [ "$DRY_RUN" -eq 1 ]; then
    printf 'Validated TLV %s -> %s\n' "$TLV_ID" "$FULL_MESSAGE_NAME"
    printf 'Would update: %s\n' "$LUA_FILE"
    printf 'Would update: %s\n' "$PROTO_FILE"
    printf 'Lua map entry: [%s] = "%s"\n' "$TLV_ID" "$FULL_MESSAGE_NAME"
    printf 'Dry run complete; no files were changed.\n'
    exit 0
fi

LUA_STAGE="$LUA_FILE.add_tlv.$$"
PROTO_STAGE="$PROTO_FILE.add_tlv.$$"

cp "$TEMP_DIR/csmp.lua.updated" "$LUA_STAGE"
cp "$TEMP_DIR/csmp.proto.updated" "$PROTO_STAGE"
chmod 0644 "$LUA_STAGE" "$PROTO_STAGE"

ROLLBACK_REQUIRED=1
mv -f "$PROTO_STAGE" "$PROTO_FILE"
PROTO_STAGE=""
mv -f "$LUA_STAGE" "$LUA_FILE"
LUA_STAGE=""

grep -Eq "^[[:space:]]*\\[$TLV_ID\\][[:space:]]*=[[:space:]]*\"$FULL_MESSAGE_NAME\"" \
    "$LUA_FILE" || fail "post-update Lua verification failed"

grep -Eq "^[[:space:]]*//[[:space:]]*TLV_ID:[[:space:]]*$TLV_ID([[:space:]]|$)" \
    "$PROTO_FILE" || fail "post-update Protobuf verification failed"

ROLLBACK_REQUIRED=0

printf 'Added TLV %s using Protobuf message %s.\n' \
    "$TLV_ID" "$FULL_MESSAGE_NAME"
printf 'Updated: %s\n' "$LUA_FILE"
printf 'Updated: %s\n' "$PROTO_FILE"
printf '%s\n' 'Reload Lua plugins or restart Wireshark to use the new TLV.'
