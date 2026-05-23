# MIPS Pipeline Simulator

A C++ project that simulates the execution of instructions in a 5-stage MIPS processor pipeline. The simulator analyzes instruction dependencies, handles data and control hazards using forwarding, stalling, and instruction reordering, then generates visual pipeline execution tables using Python.

## Project Description

This project simulates a simplified 5-stage MIPS pipeline and compares different hazard-handling techniques. It supports dependency detection, branch handling, stall insertion, forwarding, and safe instruction reordering. The simulator also produces visual tables that show how each instruction moves through the pipeline stages cycle by cycle, making it easier to analyze performance and calculate speedup.

## Features

- Simulates a 5-stage MIPS pipeline:
  - IF: Instruction Fetch
  - ID: Instruction Decode
  - EX: Execute
  - MEM: Memory Access
  - WB: Write Back
- Supports common MIPS-like instructions:
  - `add`
  - `sub`
  - `and`
  - `or`
  - `lw`
  - `sw`
  - `beq`
  - `bz`
  - `bra`
- Detects instruction dependencies:
  - RAW: Read After Write
  - WAR: Write After Read
  - WAW: Write After Write
- Handles hazards using:
  - Forwarding
  - Stalling
  - Instruction reordering
- Handles branch instructions and removes instructions that are not executed.
- Generates output files for pipeline visualization.
- Uses Python and Matplotlib to display:
  - Dependency table
  - Stall + Forwarding pipeline table
  - Forwarding + Reordering pipeline table
  - Speedup comparison

## Project Structure

```text
Final_Project/
│
├── main.cpp
├── SOLVE.hpp
├── INPUT.hpp
├── DEPENDENCIES.hpp
├── STALL_FORWARDING.hpp
├── FORWARDING_REORDERING.hpp
├── OUTPUT.hpp
│
└── visuals/
    ├── visuals.py
    ├── dependencies.txt
    ├── stall_forwarding.txt
    ├── forwarding_reordering.txt
    ├── order1.txt
    ├── order2.txt
    ├── speedUp1.txt
    └── speedUp2.txt
```

## File Explanation

### `main.cpp`

The main entry point of the project. It reads the input instructions, calculates dependencies, simulates the pipeline using different techniques, writes output files, and runs the Python visualization script.

### `INPUT.hpp`

Handles user input. It reads the number of instructions and the instruction lines entered by the user.

### `SOLVE.hpp`

Parses and transforms the input instructions into an internal format. It also simulates branch behavior and removes instructions that will not be executed because of taken branches.

### `DEPENDENCIES.hpp`

Detects dependencies between instructions, including RAW, WAR, and WAW dependencies.

### `STALL_FORWARDING.hpp`

Builds the pipeline table for the Stall + Forwarding technique. It inserts stalls when needed, especially for load-use hazards and branch-related hazards.

### `FORWARDING_REORDERING.hpp`

Builds the pipeline table for the Forwarding + Reordering technique. It attempts to safely reorder instructions to reduce stalls while preserving program correctness.

### `OUTPUT.hpp`

Writes the generated dependency and pipeline data into text files inside the `visuals` folder.

### `visuals/visuals.py`

Reads the generated output files and displays visual tables using Matplotlib. It shows dependency information, pipeline execution, and speedup results.

## Requirements

To run the project, you need:

- A C++ compiler that supports C++17 or later
- Python 3
- Matplotlib

Install Matplotlib using:

```bash
pip install matplotlib
```

## How to Run

### 1. Compile the C++ project

Using g++:

```bash
g++ main.cpp -o main
```

### 2. Run the program

```bash
./main
```

On Windows, you may run:

```bash
main.exe
```

The program will ask for the number of instructions, then ask you to enter each instruction.

## Example Input

```text
The number of instructions: 5
Instruction 1: lw $t1, 0($t0)
Instruction 2: add $t2, $t1, $t3
Instruction 3: sub $t4, $t2, $t5
Instruction 4: and $t6, $t4, $t7
Instruction 5: or $t8, $t6, $t9
```

## Output

After running the program, the project generates text files in the `visuals` folder, then runs the Python script to display the results.

Generated files include:

- `dependencies.txt`: contains detected dependencies between instructions.
- `stall_forwarding.txt`: contains the pipeline table for Stall + Forwarding.
- `forwarding_reordering.txt`: contains the pipeline table for Forwarding + Reordering.
- `order1.txt`: contains instruction order for Stall + Forwarding.
- `order2.txt`: contains instruction order for Forwarding + Reordering.
- `speedUp1.txt`: contains speedup information for Stall + Forwarding.
- `speedUp2.txt`: contains speedup information for Forwarding + Reordering.

## Visualization

The Python visualization script displays:

1. Stall + Forwarding pipeline table
2. Forwarding + Reordering pipeline table
3. Instruction dependency table
4. Speedup values for both techniques

Each row represents an instruction, and each column represents a clock cycle. Pipeline stages are shown as:

- `IF`
- `ID`
- `EX`
- `MEM`
- `WB`
- `STALL`

## Supported Instruction Format

Use the following formats:

```text
add $t1, $t2, $t3
sub $t1, $t2, $t3
and $t1, $t2, $t3
or $t1, $t2, $t3
lw $t1, 0($t2)
sw $t1, 0($t2)
beq $t1, $t2, label
bz $t1, label
bra label
```

Labels are written before an instruction using `:`:

```text
loop: add $t1, $t2, $t3
```

## Notes

- The maximum number of instructions is 256.
- The simulator uses a simplified MIPS model for educational purposes.
- The project focuses on pipeline behavior, dependency analysis, hazard handling, and visualization.
- Instruction reordering is performed only when it is safe and does not break dependency correctness.

## Technologies Used

- C++
- Python
- Matplotlib

## Author

Developed as a Computer Architecture / MIPS Pipeline Simulation project.
