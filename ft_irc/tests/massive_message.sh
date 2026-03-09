#!/bin/bash

# Configuration
HOST="localhost"
PORT="8080"
PASS="test"      # Change this to your server password
CHANNEL="#ok"
NICK="FlooderBot"

# Generate a massive payload (2000 characters of 'A')
# This exceeds the 512-byte limit, triggering your Flood Protection.
PAYLOAD=$(printf 'A%.0s' {1..2000})

echo "--- Launching Flood Test on $CHANNEL ---"

(
    # 1. Handshake
    echo "PASS $PASS"
    echo "NICK $NICK"
    echo "USER flood 0 * :Flood Bot"
    sleep 0.5

    # 2. Join the channel
    echo "JOIN $CHANNEL"
    sleep 0.5

    # 3. Send the death message
    echo "PRIVMSG $CHANNEL :$PAYLOAD"
    
    # 4. Wait to see the error message from server
    sleep 1
) | nc -C $HOST $PORT

echo -e "\n--- Test Finished ---"
