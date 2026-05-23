#ifndef DEPENDENCIES_HPP
#define DEPENDENCIES_HPP

// Functions' Headers.

// To make the dependencies vector ready.
inline void calculateDependencies();

// Functions' Bodies.

// To make the dependencies vector ready.
inline void calculateDependencies() {
	// Stores up to 2 source register addresses for each instruction (-1 if none).
	vector<array<int, 2>> sources(tempInstructions.size(), { -1, -1 });

	// Stores the destination register address for each instruction (-1 if none).
	vector<int> destination(tempInstructions.size(), -1);

	for (int i = 0; i < tempInstructions.size(); ++i) {
		if (tempInstructions[i][1] <= 3) {
			destination[i] = tempInstructions[i][2];

			sources[i][0] = tempInstructions[i][3];
			sources[i][1] = tempInstructions[i][4];
		}

		else if (tempInstructions[i][1] == 4) {
			destination[i] = tempInstructions[i][2];

			sources[i][0] = tempInstructions[i][4];
		}

		else if (tempInstructions[i][1] == 5) {
			sources[i][0] = tempInstructions[i][2];
			sources[i][1] = tempInstructions[i][4];
		}

		else if (tempInstructions[i][1] == 6) {
			sources[i][0] = tempInstructions[i][2];
			sources[i][1] = tempInstructions[i][3];
		}

		else if (tempInstructions[i][1] == 7) {
			sources[i][0] = tempInstructions[i][2];
		}
	}

	for (int i = 0; i < tempInstructions.size(); ++i) {
		for (int j = i - 1; ~j; --j) {
			if (destination[j] != -1) {
				if (sources[i][0] == destination[j]) {
					dependencies.push_back({ "I" + to_string(i + 1), "I" + to_string(j + 1), "$" + reverseRegisterAddress[sources[i][0]], "RAW" });
				}

				else if (sources[i][1] == destination[j]) {
					dependencies.push_back({ "I" + to_string(i + 1), "I" + to_string(j + 1), "$" + reverseRegisterAddress[sources[i][1]], "RAW" });
				}

				if (destination[i] == destination[j]) {
					dependencies.push_back({ "I" + to_string(i + 1), "I" + to_string(j + 1), "$" + reverseRegisterAddress[destination[i]], "WAW" });
				}
			}
		}
	}

	for (int i = 0; i < tempInstructions.size(); ++i) {
		for (int j = i + 1; j < tempInstructions.size(); ++j) {
			if (destination[j] != -1) {
				if (destination[j] == sources[i][0]) {
					dependencies.push_back({ "I" + to_string(j + 1), "I" + to_string(i + 1), "$" + reverseRegisterAddress[sources[i][0]], "WAR" });
				}

				else if (destination[j] == sources[i][1]) {
					dependencies.push_back({ "I" + to_string(j + 1), "I" + to_string(i + 1), "$" + reverseRegisterAddress[sources[i][1]], "WAR" });
				}
			}
		}
	}

	sort(dependencies.begin(), dependencies.end());
}

#endif