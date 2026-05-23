#ifndef OUTPUT_HPP
#define OUTPUT_HPP

// Global Variables.

// To output to files.
ofstream outputFile;

// Functions' Headers.

// To output the dependencies table.
inline void outputDependencies();

// To output stall_forwarding table.
inline void outputStallForwarding();

// TO output forwarding_reordering table.
inline void outputForwardingReordering();

// Functions' Bodies.

inline void outputDependencies() {
	outputFile.open("visuals/dependencies.txt");
	for (int i = 0; i < dependencies.size(); ++i) {
		for (int j = 0; j < 4; ++j) {
			outputFile << dependencies[i][j] << ' ';
		}

		outputFile << endl;
	}

	outputFile.close();
}

inline void outputStallForwarding() {
	outputFile.open("visuals/stall_forwarding.txt");
	for (int i = 0; i < stall_forwarding.size(); ++i) {
		for (int j = 0; j < stall_forwarding[0].size(); ++j) {
			outputFile << stall_forwarding[i][j] << ' ';
		}

		outputFile << endl;
	}

	outputFile.close();

	outputFile.open("visuals/order1.txt");
	for (int i = 0; i < order1.size(); ++i) {
		outputFile << order1[i] << ' ';
	}

	outputFile << endl;
	outputFile.close();

	outputFile.open("visuals/speedUp1.txt");
	outputFile << 5 * tempInstructions.size() << ' ' << stall_forwarding[0].size() << endl;
	outputFile.close();
}

inline void outputForwardingReordering() {
	outputFile.open("visuals/forwarding_reordering.txt");
	for (int i = 0; i < forwarding_reordering.size(); ++i) {
		for (int j = 0; j < forwarding_reordering[0].size(); ++j) {
			outputFile << forwarding_reordering[i][j] << ' ';
		}

		outputFile << endl;
	}

	outputFile.close();

	outputFile.open("visuals/order2.txt");
	for (int i = 0; i < order2.size(); ++i) {
		outputFile << order2[i] << ' ';
	}

	outputFile << endl;
	outputFile.close();

	outputFile.open("visuals/speedUp2.txt");
	outputFile << 5 * tempInstructions.size() << ' ' << forwarding_reordering[0].size() << endl;
	outputFile.close();
}
#endif