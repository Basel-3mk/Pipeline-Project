#ifndef STALL_FORWARDING_HPP
#define STALL_FORWARDING_HPP

// Functions' Headers.

// This function will fill up stall_forwarding table.
inline void calculateStallForwarding();

// Functions' Bodies.

inline void calculateStallForwarding() {
	// Stores up to 2 source register addresses for each instruction (-1 if none).
	vector<array<int, 2>> sources(instructionsNumber, { -1, -1 });

	// Stores the destination register address for each instruction (-1 if none).
	vector<int> destination(instructionsNumber, -1);

	for (int i = 0; i < instructionsNumber; ++i) {
		if (instructions[i][1] <= 3) {
			destination[i] = instructions[i][2];

			sources[i][0] = instructions[i][3];
			sources[i][1] = instructions[i][4];
		}

		else if (instructions[i][1] == 4) {
			destination[i] = instructions[i][2];

			sources[i][0] = instructions[i][4];
		}

		else if (instructions[i][1] == 5) {
			sources[i][0] = instructions[i][2];
			sources[i][1] = instructions[i][4];
		}

		else if (instructions[i][1] == 6) {
			sources[i][0] = instructions[i][2];
			sources[i][1] = instructions[i][3];
		}

		else if (instructions[i][1] == 7) {
			sources[i][0] = instructions[i][2];
		}
	}

	for (int i = 0; i < instructionsNumber; ++i) {
		stall_forwarding.push_back({});
		order1.push_back(oldNumber[i]);
		if (i) {
			if ((instructions[i - 1][1] == 4 and (sources[i][0] == destination[i - 1] or sources[i][1] == destination[i - 1])) or (instructions[i - 1][1] >= 6 and (isAfter[oldNumber[i - 1] - 1] == true or isTaken[oldNumber[i - 1] - 1] == false))) {
				int j = 0;
				while (true) {
					if (stall_forwarding[stall_forwarding.size() - 2][j] == 0) {
						j++;
						break;
					}

					else {
						if (stall_forwarding[stall_forwarding.size() - 2][j] == -1) {
							stall_forwarding.back().push_back(-1);
						}

						else {
							stall_forwarding.back().push_back(-2);
						}

						j++;
					}
				}

				stall_forwarding.back().push_back(-2);

				for (int k = 0; k < 1; ++k) {
					if (stall_forwarding[stall_forwarding.size() - 2].size() <= j) {
						stall_forwarding.back().push_back(k);
						j++;
					}

					else {
						if (stall_forwarding[stall_forwarding.size() - 2][j] == -1) {
							stall_forwarding.back().push_back(-1);
							j++;
							k--;
						}

						else {
							stall_forwarding.back().push_back(k);
							j++;
						}
					}
				}

				while (stall_forwarding[stall_forwarding.size() - 2].size() > j and stall_forwarding[stall_forwarding.size() - 2][j] == -1) {
					stall_forwarding.back().push_back(-1);
					j++;
				}

				stall_forwarding.back().push_back(-1);
				j++;

				for (int k = 1; k < 5; ++k) {
					if (stall_forwarding[stall_forwarding.size() - 2].size() <= j) {
						stall_forwarding.back().push_back(k);
						j++;
					}

					else {
						if (stall_forwarding[stall_forwarding.size() - 2][j] == -1) {
							stall_forwarding.back().push_back(-1);
							j++;
							k--;
						}

						else {
							stall_forwarding.back().push_back(k);
							j++;
						}
					}
				}
			}

			else {
				int j = 0;
				while (true) {
					if (stall_forwarding[stall_forwarding.size() - 2][j] == 0) {
						j++;
						break;
					}

					else {
						if (stall_forwarding[stall_forwarding.size() - 2][j] == -1) {
							stall_forwarding.back().push_back(-1);
						}

						else {
							stall_forwarding.back().push_back(-2);
						}
					}

					j++;
				}

				stall_forwarding.back().push_back(-2);

				for (int k = 0; k < 5; ++k) {
					if (stall_forwarding[stall_forwarding.size() - 2].size() <= j) {
						stall_forwarding.back().push_back(k);
						j++;
					}

					else {
						if (stall_forwarding[stall_forwarding.size() - 2][j] == -1) {
							stall_forwarding.back().push_back(-1);
							j++;
							k--;
						}

						else {
							stall_forwarding.back().push_back(k);
							j++;
						}
					}
				}
			}
		}

		else {
			for (int j = 0; j < 5; ++j) {
				stall_forwarding.back().push_back(j);
			}
		}

		if (instructions[i][1] >= 6) {
			if (!isAfter[oldNumber[i] - 1]) {
				if (isTaken[oldNumber[i] - 1]) {
					stall_forwarding.push_back({});
					order1.push_back(oldNumber[i] + 1);

					int curPointer = 0;
					while (stall_forwarding[stall_forwarding.size() - 2][curPointer] != 0) {
						stall_forwarding.back().push_back(stall_forwarding[stall_forwarding.size() - 2][curPointer]);
						curPointer++;
					}

					stall_forwarding.back().push_back(-2);
					curPointer++;

					while (stall_forwarding[stall_forwarding.size() - 2][curPointer] == -1) {
						stall_forwarding.back().push_back(-1);
						curPointer++;
					}

					stall_forwarding.back().push_back(0);
					stall_forwarding.back().push_back(-1);
				}
			}
		}
	}

	int mx_cycles = 0;
	for (int i = 0; i < stall_forwarding.size(); ++i) {
		mx_cycles = max(mx_cycles, (int)stall_forwarding[i].size());
	}

	for (int i = 0; i < stall_forwarding.size(); ++i) {
		while (stall_forwarding[i].size() < mx_cycles) {
			stall_forwarding[i].push_back(-2);
		}
	}

	for (int i = 0; i < mx_cycles; ++i) {
		for (int j = stall_forwarding.size() - 1; j; --j) {
			if (stall_forwarding[j - 1][i] == -1) {
				stall_forwarding[j][i] = -2;
			}

			else {
				break;
			}
		}
	}
}

#endif