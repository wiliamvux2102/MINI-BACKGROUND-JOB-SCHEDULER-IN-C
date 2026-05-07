# Mini Background Job Scheduler

A multi-threaded job scheduler written in C that simulates background job processing using configurable worker pools and multiple scheduling policies.

## Features

- **Three Scheduling Algorithms:**
  - FIFO (First-In-First-Out)
  - SJF (Shortest Job First)
  - Priority-based scheduling

- **Thread-safe Architecture:**
  - Worker pool with configurable number of threads
  - Mutex and condition variable synchronization
  - Concurrent job processing

- **CSV-based Job Input:**
  - Parse jobs from CSV files
  - Support for up to 200 jobs
  - Job metadata includes ID, seller, arrival time, runtime, priority, and type

- **Metrics and Logging:**
  - Real-time simulation time tracking
  - Worker activity logging
  - Summary statistics (turnaround time, throughput)

## Project Structure

```
.
├── include/
│   ├── job.h          # Job structure and status definitions
│   ├── metrics.h      # Metrics calculation functions
│   ├── parser.h       # CSV parsing declarations
│   ├── queue.h        # Queue data structure and operations
│   ├── scheduler.h    # Scheduler interface and policies
│   └── worker.h       # Worker thread definitions
├── src/
│   ├── main.c         # Entry point and orchestration
│   ├── scheduler.c    # Core scheduling logic
│   ├── queue.c        # Queue implementation (FIFO, SJF, Priority)
│   ├── worker.c       # Worker thread pool
│   ├── parser.c       # CSV file parser
│   ├── logger.c       # Simulation time and logging
│   └── metrics.c      # Performance metrics
├── Makefile           # Build configuration
└── README.md          # This file
```

## Building

```bash
make
```

This compiles all source files and produces the `scheduler` executable.

To clean build artifacts:

```bash
make clean
```

## Usage

```bash
./scheduler <jobs.csv> <policy> <num_workers>
```

### Arguments

- `jobs.csv`: Path to CSV file containing job definitions
- `policy`: Scheduling policy - `fifo`, `sjf`, or `priority`
- `num_workers`: Number of worker threads to create

### CSV Format

The CSV file should have the following format (header required):

```csv
job_id,seller_id,arrival_time,estimated_runtime,priority,job_type
1,SELLER_A,0,5,3,report_generation
2,SELLER_B,2,3,1,data_export
3,SELLER_C,5,8,2,analytics
```

### Example

```bash
./scheduler jobs.csv sjf 4
```

This runs the scheduler with:
- Jobs from `jobs.csv`
- Shortest Job First policy
- 4 worker threads

## Scheduling Policies

### FIFO (First-In-First-Out)
Jobs are processed in the order they arrive at the ready queue.

### SJF (Shortest Job First)
Jobs are sorted by estimated runtime, with shorter jobs processed first.

### Priority
Jobs are sorted by priority value (lower values = higher priority).

## Output

The scheduler outputs:
1. Real-time logs showing worker activity:
   ```
   [time=0] Worker 0 starts Job 1 seller=SELLER_A runtime=5 priority=3
   [time=5] Worker 0 finishes Job 1 seller=SELLER_A runtime=5 priority=3
   ```

2. Summary metrics at completion:
   - Total jobs processed
   - Average turnaround time
   - Average waiting time
   - System throughput
   - Worker utilization

## Requirements

- GCC compiler
- POSIX threads (pthread) library
- Linux or Unix-like system (uses `clock_gettime`)

## License

This is an educational project for Operating Systems coursework.
