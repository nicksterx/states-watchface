#!/bin/bash

# Capture screenshots for specific minutes

PLATFORM=${1:-basalt}
OUTPUT_DIR="screenshots"

# Minutes to capture
MINUTES=(0 1 13 31 50 57 58 59)

echo "========================================="
echo "Pebble Watchface Screenshot Capture"
echo "Representative Samples"
echo "========================================="
echo ""
echo "Platform: $PLATFORM"
echo "Will capture minutes: ${MINUTES[@]}"
echo ""

# Create output directory
mkdir -p "$OUTPUT_DIR"

# Install to emulator
echo "Installing watchface to emulator..."
pebble install --emulator $PLATFORM

if [ $? -ne 0 ]; then
    echo ""
    echo "Error: Failed to install to emulator"
    echo "Please start the emulator first with:"
    echo "  pebble install --emulator $PLATFORM"
    exit 1
fi

echo ""
echo "Emulator ready. Waiting for target minutes..."
echo "Press Ctrl+C to stop at any time."
echo ""

# Track which minutes we've captured using a simple file-based approach
CAPTURED_FILE="${OUTPUT_DIR}/.captured"
> "$CAPTURED_FILE"  # Clear the file

captured_count() {
    if [ -f "$CAPTURED_FILE" ]; then
        wc -l < "$CAPTURED_FILE" | tr -d ' '
    else
        echo "0"
    fi
}

is_captured() {
    local minute=$1
    if [ -f "$CAPTURED_FILE" ]; then
        grep -q "^${minute}$" "$CAPTURED_FILE"
        return $?
    fi
    return 1
}

mark_captured() {
    local minute=$1
    echo "$minute" >> "$CAPTURED_FILE"
}

while true; do
    CURRENT_MINUTE=$(date +%M | sed 's/^0*//')  # Remove leading zeros
    if [ -z "$CURRENT_MINUTE" ]; then
        CURRENT_MINUTE=0
    fi

    # Check if this is a target minute and not yet captured
    for target in "${MINUTES[@]}"; do
        if [ "$CURRENT_MINUTE" -eq "$target" ]; then
            if ! is_captured "$target"; then
                FILENAME="${OUTPUT_DIR}/minute_$(printf '%02d' $target)_${PLATFORM}.png"

                echo "[$(date +%H:%M:%S)] Capturing minute :$(printf '%02d' $target)"
                pebble screenshot --emulator $PLATFORM "$FILENAME"

                if [ $? -eq 0 ]; then
                    echo "  ✓ Saved: $FILENAME"
                    mark_captured "$target"

                    COUNT=$(captured_count)
                    # Check if we're done
                    if [ "$COUNT" -eq "${#MINUTES[@]}" ]; then
                        echo ""
                        echo "========================================="
                        echo "All screenshots captured!"
                        echo "Images saved to: $OUTPUT_DIR/"
                        echo "========================================="
                        ls -lh "$OUTPUT_DIR"/*.png
                        rm -f "$CAPTURED_FILE"
                        exit 0
                    fi
                else
                    echo "  ✗ Failed to capture screenshot"
                fi
            fi
        fi
    done

    # Show progress
    COUNT=$(captured_count)
    echo -ne "\r[${COUNT}/${#MINUTES[@]} captured] Current time: $(date +%H:%M:%S) - Waiting...    "

    sleep 5
done
