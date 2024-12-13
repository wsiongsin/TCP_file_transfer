#!/bin/bash

FILE="smallTest.txt"
SERVER="linux-01.socs.uoguelph.ca:50958"
BUFFER_SIZE=4096
NUM_TRANSFERS=20
TIMES=()

for i in $(seq 1 $NUM_TRANSFERS); do
    START=$(date +%s%N)  # Start time in nanoseconds
    ./sendFile "$FILE" "$SERVER" "$BUFFER_SIZE"
    END=$(date +%s%N)    # End time in nanoseconds
    TIME=$((END - START))
    TIMES+=($TIME)
    echo "Transfer $i took $((TIME / 1000000)) ms"
done

# Calculate min, max, and average transfer times
MIN=${TIMES[0]}
MAX=${TIMES[0]}
SUM=0

for TIME in "${TIMES[@]}"; do
    (( TIME < MIN )) && MIN=$TIME
    (( TIME > MAX )) && MAX=$TIME
    SUM=$((SUM + TIME))
done

AVG=$((SUM / NUM_TRANSFERS))

echo "Minimum time: $((MIN / 1000000)) ms"
echo "Maximum time: $((MAX / 1000000)) ms"
echo "Average time: $((AVG / 1000000)) ms"

