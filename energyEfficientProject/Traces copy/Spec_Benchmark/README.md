# Cache Simulator README

## Overview

This is a cache simulator meant to demonstrate how cache and 
memory accesses affect the static and dynamic energy 
of a computer system. In our case a simple computer system with an L1 instruction cache, L1 data cache, L2 unified cache 
and DRAM. It utilizes a combination of Python scripts and a Bash script to run simulations and analyze the results redirected to a text file called formatted_output.txt. 

## Features

- Simulates cache memory behavior based on associativity 
- Provides statistics such as cache hits, misses, energy consumption, mean, and standard deviation.
- Supports customizable cache configurations including cache size, associativity, and block size.

## Prerequisites

- Python 3.x
- Bash shell
- mean_and_std.py: Python script for calculating mean and standard deviation
- with_penalties.py: Python script to run memory traces on a simulated memory system

## Usage

### Running the Cache Simulator

1. Set up the input trace files in the desired format. Each line in the trace file represents a memory access.

2. Configure the cache parameters in the `run.sh` script.

3. Execute the cache simulator by running the `run.sh` script:

   ```bash
   bash run.sh
