#include <algorithm>
#include <array>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

#define endl '\n'

// Global Variables.

// The maximum number of instructions.
const int MAX_INSTRUCTIONS = 256;

// The operations {The name of the operation, The index of the operation}.
const map<string, int> operations = {{"add", 0}, {"sub", 1}, {"and", 2}, {"or", 3}, {"lw", 4}, {"sw", 5}, {"beq", 6}, {"bz", 7}, {"bra", 8}};

// The type of each operation (0 -> destination, source1, source2), (1 -> source/destination, offset(base)), (2 -> source1, source2, targetLabel), (3 -> source1, targetLabel), (4 -> targetLabel).
const vector<int> operationType = {0, 0, 0, 0, 1, 1, 2, 3, 4};

// The instruction vector that describes each instruction {Label Address (It's -1 if there's no label), Operation number, The 3 operands of the operation}.
vector<array<int, 5>> instructions;

// The address of each used register.
map<string, int> registerAddress;

// The name of each register from it's memory address.
map<int, string> reverseRegisterAddress;

// The address of each used label (after mapping the label).
map<int, int> labelAddress = {{-1, -1}};

// Transform labels from string to int, to achieve forward branch for some labels.
map<string, int> labelMapping = {{"", -1}};

// The number of total instructions that the user enters.
int instructionsNumber;

// The input as vector of strings, each string is an input line.
vector<string> instructionsString;

// This map tells whether a branch instruction is taken or not during execution.
map<int, bool> isTaken;

// This map tells whether a targetLabel comes directly after it's branch or not.
map<int, bool> isAfter;

// The order of remaining instructions after deletion before any delete operation.
vector<int> oldNumber;

// The file which contains Solve() function.
#include "headers/SOLVE.hpp"

// The file which contains Input() function.
#include "headers/INPUT.hpp"

// (Ii, Ij, Register) that instruction Ii depends on instruction Ij on $Register, we need it for visualization.
vector<array<string, 4>> dependencies;

// The file which contains calculateDependencies() function.
#include "headers/DEPENDENCIES.hpp"

// The instructions' order in stall_forwarding.
vector<int> order1;

// A 2D-Matrix, each row is the instruction, each column is one clock cycle, we need it for visualization, (-1 -> Stall), (-2 -> Empty).
vector<vector<int>> stall_forwarding;

// The file which contains calculateStallForwarding() function.
#include "headers/STALL_FORWARDING.hpp"

// A 2D-Matrix, each row is the instruction, each column is one clock cycle, we need it for visualization, (-1 -> Stall), (-2 -> Empty).
vector<vector<int>> forwarding_reordering;

// The instructions' order in forwarding_reordering.
vector<int> order2;

// The file which contains calculateForwardingReordering() function.
#include "headers/FORWARDING_REORDERING.hpp"

// To output required outputs to specific files.
#include "headers/OUTPUT.hpp"

// Main.
int main() {
  Input();

  // Dependencies.
  calculateDependencies();
  outputDependencies();

  // Stall + Forwarding.
  calculateStallForwarding();
  outputStallForwarding();

  // Forwarding + Reordering.
  calculateForwardingReordering();
  outputForwardingReordering();

  // Run the visual script (Python).
  system("python visuals\\visuals.py");

  // Thank God.
  return 0;
}