#!/bin/bash

# Script to capture screenshots of each minute for the Pebble watchface
# Usage: ./capture_screenshots.sh [platform]
# Default platform: basalt

PLATFORM=${1:-basalt}
OUTPUT_DIR="screenshots"

echo "Starting Pebble emulator for $PLATFORM..."
echo "This script will capture screenshots for minutes 0-59"
echo ""

# Install the watchface to the emulator
pebble install --emulator $PLATFORM

echo ""
echo "Please manually set the time in the emulator to display each minute"
echo "and run: pebble screenshot --emulator $PLATFORM screenshots/minute_XX.png"
echo ""
echo "Alternative: Use the timeline to set specific times, but this requires manual work."
echo ""
echo "Unfortunately, the Pebble SDK doesn't provide an automated way to set the"
echo "system time in the emulator, so you'll need to either:"
echo ""
echo "1. Manually change your system time and take screenshots"
echo "2. Wait for each minute to pass (takes 60 minutes)"
echo "3. Use the pebble screenshot command repeatedly as time passes"
echo ""
echo "Screenshot command:"
echo "  pebble screenshot --emulator $PLATFORM $OUTPUT_DIR/minute_\$(date +%M).png"
