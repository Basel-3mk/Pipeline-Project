#ifndef INPUT_HPP
#define INPUT_HPP

// Functions' Headers.

// To input the instructions and transform them into vectors and maps to deal with them.
inline void Input();

// Functions' Bodies.

inline void Input() {
	while (true) {
		cout << "The number of instructions: ";
		cin >> instructionsNumber;

		if (instructionsNumber > MAX_INSTRUCTIONS) {
			cout << "The maximum number of instructions is " << MAX_INSTRUCTIONS << " instruction. Try again!" << endl;
		}

		else {
			break;
		}
	}

	instructionsString.assign(instructionsNumber, {});

	cin.ignore();
	for (int i = 0; i < instructionsNumber; ++i) {
		cout << "Instruction " << i + 1 << ": ";
		string curInstruction;
		getline(cin, curInstruction);
		instructionsString[i] = curInstruction;
	}

	Solve();
}

#endif