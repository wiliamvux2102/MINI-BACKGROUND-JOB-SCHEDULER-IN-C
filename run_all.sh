#!/bin/bash

# --- Configuration ---
TARGET="./scheduler"
LOG_DIR="logs"
WLD_DIR="workloads"
WORKERS=4
WORKLOADS=("workload_a" "workload_b" "workload_c")
POLICIES=("fifo" "sjf" "priority")

# 1. Compile using Makefile
echo "[+] Building project using Makefile..."
make clean && make

if [ $? -ne 0 ]; then
    echo "[-] Compilation failed. Please check the source code and Makefile."
    exit 1
fi

# 2. Create logs directory
mkdir -p $LOG_DIR

echo "[+] Starting simulations and logging..."
echo "------------------------------------------------"

# 3. Loop through each scenario
for wl in "${WORKLOADS[@]}"; do
    for pol in "${POLICIES[@]}"; do
        
        CSV_PATH="$WLD_DIR/$wl.csv"
        LOG_FILE="$LOG_DIR/${wl}_${pol}.log"

        if [ ! -f "$CSV_PATH" ]; then
            echo "[-] Skipping: $CSV_PATH not found"
            continue
        fi

        echo -n "    - Running: $wl + $pol... "

        # Run and redirect stdout/stderr to log file
        $TARGET "$CSV_PATH" "$pol" $WORKERS > "$LOG_FILE" 2>&1

        if [ $? -eq 0 ]; then
            echo "Done! (Saved to $LOG_FILE)"
        else
            echo "Failed!"
        fi
    done
done

echo "------------------------------------------------"
echo "[+] All simulations completed. Logs are saved in '$LOG_DIR/'."