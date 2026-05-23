#ifndef SOLVE_HPP
#define SOLVE_HPP

// Global Variables.

// The number of registers of each type.
const int registersNum = 250;

// The t-registers' array.
vector<int> tRegisters(registersNum);

// Instructions array before removing the instructions that won't happen.
vector<array<int, 5>> tempInstructions;

// Memory size.
const int memorySize = 2048;

// The memory (for lw and sw).
vector<int> memory(memorySize);

// The cur address that is ready to store in memory (means it's empty at the moment).
int curAddress;

// Functions' Headers.

// To solve the branches, and then remove the instructions that won't happen because of branches.
inline void Solve();

// To put each instruction in the format of instructions vector.
inline array<int, 5> Transform(string&, int);

// It transforms register num from string to int.
inline int transformToInt(int);

// Functions' Bodies.
inline void Solve() {
	oldNumber.assign(instructionsNumber, 0);
	curAddress = instructionsNumber;

	// Setting initial values 0, 1, 2, ...
	iota(tRegisters.begin(), tRegisters.end(), 0);
	iota(memory.begin(), memory.end(), 0);

	for (int i = 0; i < instructionsNumber; ++i) {
		tempInstructions.push_back(Transform(instructionsString[i], i));
	}

	for (int i = 0; i < instructionsNumber; ++i) {
		if (tempInstructions[i][1] >= 6) {
			tempInstructions[i][4] = labelAddress[tempInstructions[i][4]];
		}
	}

	// Simulation.
	for (int i = 0; i < instructionsNumber; ++i) {
		switch (tempInstructions[i][1]) {
		case 0: {
			tRegisters[transformToInt(tempInstructions[i][2])] = tRegisters[transformToInt(tempInstructions[i][3])] + tRegisters[transformToInt(tempInstructions[i][4])];
		} break;

		case 1: {
			tRegisters[transformToInt(tempInstructions[i][2])] = tRegisters[transformToInt(tempInstructions[i][3])] - tRegisters[transformToInt(tempInstructions[i][4])];
		} break;

		case 2: {
			tRegisters[transformToInt(tempInstructions[i][2])] = tRegisters[transformToInt(tempInstructions[i][3])] & tRegisters[transformToInt(tempInstructions[i][4])];
		} break;

		case 3: {
			tRegisters[transformToInt(tempInstructions[i][2])] = tRegisters[transformToInt(tempInstructions[i][3])] | tRegisters[transformToInt(tempInstructions[i][4])];
		} break;

		case 4: {
			tRegisters[transformToInt(tempInstructions[i][2])] = memory[tRegisters[transformToInt(tempInstructions[i][4])] + tempInstructions[i][3]];
		} break;

		case 5: {
			memory[tRegisters[transformToInt(tempInstructions[i][4])] + tempInstructions[i][3]] = tRegisters[transformToInt(tempInstructions[i][2])];
		} break;

		case 6: {
			if (tRegisters[transformToInt(tempInstructions[i][2])] == tRegisters[transformToInt(tempInstructions[i][3])]) {
				isTaken[i] = true;
				i = tempInstructions[i][4] - 1;
			}

			else {
				isTaken[i] = false;
			}
		} break;

		case 7: {
			if (tRegisters[transformToInt(tempInstructions[i][2])] == 0) {
				isTaken[i] = true;
				i = tempInstructions[i][4] - 1;
			}

			else {
				isTaken[i] = false;
			}
		} break;

		case 8: {
			isTaken[i] = true;
			i = tempInstructions[i][4] - 1;
		} break;
		}
	}

	// Filling isAfter.
	for (int i = 0; i < instructionsNumber; ++i) {
		if (tempInstructions[i][1] >= 6) {
			if (tempInstructions[i][4] == i + 1) {
				isAfter[i] = true;
			}

			else {
				isAfter[i] = false;
			}
		}
	}

	// Delete unwanted instructions.
	for (int i = 0; i < instructionsNumber; ++i) {
		instructions.push_back(tempInstructions[i]);
		oldNumber[instructions.size() - 1] = i + 1;

		if (tempInstructions[i][1] >= 6) {
			if (isTaken[i]) {
				i = tempInstructions[i][4] - 1;
			}
		}
	}

	instructionsNumber = instructions.size();
}

inline array<int, 5> Transform(string& curInstruction, int curInstructionOrder) {
	// stringstream is like cin, but the given string (curInstruction) will be as a line in cin, but instead of cin it will be stringstream.
	stringstream ss(curInstruction);

	string label;
	ss >> label;

	// The label ends with ':', if not, then it's operation.
	string operation;
	if (label.back() != ':') {
		operation = label;

		label.clear();
	}

	else {
		// To remove ':'.
		label.pop_back();

		ss >> operation;
	}

	int operationIndex = operations.find(operation)->second;

	// add, sub, and, or.
	if (operationType[operationIndex] == 0) {
		string destination, source1, source2;
		ss >> destination >> source1 >> source2;

		// To remove '$'.
		destination.erase(0, 1);
		source1.erase(0, 1);
		source2.erase(0, 1);

		// To remove ',';
		destination.pop_back();
		source1.pop_back();

		// Assign addresses.
		if (label.empty() == false) {
			if (labelMapping.count(label) == 0) {
				labelMapping[label] = labelMapping.size();
			}

			labelAddress[labelMapping[label]] = curInstructionOrder;
		}

		if (registerAddress.count(destination) == 0) {
			registerAddress[destination] = curAddress;
			reverseRegisterAddress[curAddress] = destination;
			curAddress++;
		}

		if (registerAddress.count(source1) == 0) {
			registerAddress[source1] = curAddress;
			reverseRegisterAddress[curAddress] = source1;
			curAddress++;
		}

		if (registerAddress.count(source2) == 0) {
			registerAddress[source2] = curAddress;
			reverseRegisterAddress[curAddress] = source2;
			curAddress++;
		}

		return { labelAddress[labelMapping[label]], operationIndex, registerAddress[destination], registerAddress[source1], registerAddress[source2] };
	}

	// lw, sw.
	else if (operationType[operationIndex] == 1) {
		string destination, base;
		int offset;
		ss >> destination >> offset >> base;

		// To remove '('.
		base.erase(0, 1);

		// To remove ')'.
		base.pop_back();

		// To remove '$'.
		destination.erase(0, 1);
		base.erase(0, 1);

		// To remove ','.
		destination.pop_back();

		// Assign addresses.
		if (label.empty() == false) {
			if (labelMapping.count(label) == 0) {
				labelMapping[label] = labelMapping.size();
			}

			labelAddress[labelMapping[label]] = curInstructionOrder;
		}

		if (registerAddress.count(destination) == 0) {
			registerAddress[destination] = curAddress;
			reverseRegisterAddress[curAddress] = destination;
			curAddress++;
		}

		if (registerAddress.count(base) == 0) {
			registerAddress[base] = curAddress;
			reverseRegisterAddress[curAddress] = base;
			curAddress++;
		}

		return { labelAddress[labelMapping[label]], operationIndex, registerAddress[destination], offset, registerAddress[base] };
	}

	// beq.
	else if (operationType[operationIndex] == 2) {
		string source1, source2, targetLabel;
		ss >> source1 >> source2 >> targetLabel;

		// To remove '$'.
		source1.erase(0, 1);
		source2.erase(0, 1);

		// To remove ','.
		source1.pop_back();
		source2.pop_back();

		// Assign addresses.
		if (label.empty() == false) {
			if (labelMapping.count(label) == 0) {
				labelMapping[label] = labelMapping.size();
			}

			labelAddress[labelMapping[label]] = curInstructionOrder;
		}

		if (registerAddress.count(source1) == 0) {
			registerAddress[source1] = curAddress;
			reverseRegisterAddress[curAddress] = source1;
			curAddress++;
		}

		if (registerAddress.count(source2) == 0) {
			registerAddress[source2] = curAddress;
			reverseRegisterAddress[curAddress] = source2;
			curAddress++;
		}

		if (labelMapping.count(targetLabel) == 0) {
			labelMapping[targetLabel] = labelMapping.size();
		}

		return { labelAddress[labelMapping[label]], operationIndex, registerAddress[source1], registerAddress[source2], labelMapping[targetLabel] };
	}

	// bz.
	else if (operationType[operationIndex] == 3) {
		string source1, targetLabel;
		ss >> source1 >> targetLabel;

		// To remove '$'.
		source1.erase(0, 1);

		// To remove ','.
		source1.pop_back();

		// Assign addresses.
		if (label.empty() == false) {
			if (labelMapping.count(label) == 0) {
				labelMapping[label] = labelMapping.size();
			}

			labelAddress[labelMapping[label]] = curInstructionOrder;
		}

		if (registerAddress.count(source1) == 0) {
			registerAddress[source1] = curAddress;
			reverseRegisterAddress[curAddress] = source1;
			curAddress++;
		}

		if (labelMapping.count(targetLabel) == 0) {
			labelMapping[targetLabel] = labelMapping.size();
		}

		return { labelAddress[labelMapping[label]], operationIndex, registerAddress[source1], -1, labelMapping[targetLabel] };
	}

	// br.
	else {
		string targetLabel;
		ss >> targetLabel;

		// Assign addresses.
		if (label.empty() == false) {
			if (labelMapping.count(label) == 0) {
				labelMapping[label] = labelMapping.size();
			}

			labelAddress[labelMapping[label]] = curInstructionOrder;
		}

		if (labelMapping.count(targetLabel) == 0) {
			labelMapping[targetLabel] = labelMapping.size();
		}

		return { labelAddress[labelMapping[label]], operationIndex, -1, -1, labelMapping[targetLabel] };
	}
}

inline int transformToInt(int curRegisterAddress) {
	// Get the register name.
	string curRegister = reverseRegisterAddress[curRegisterAddress];

	// Remove t.
	curRegister.erase(0, 1);

	// Transform to int.
	return stoi(curRegister);
}

#endif