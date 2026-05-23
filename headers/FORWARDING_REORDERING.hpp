#ifndef FORWARDING_REORDERING_HPP
#define FORWARDING_REORDERING_HPP

// Functions' Headers.

// This function will fill up forwarding_reordering table.
inline void calculateForwardingReordering();

// Functions' Bodies.

inline void calculateForwardingReordering() {
	vector<int> tempOrder(instructionsNumber);

	// It will make the tempOrder = {0, 1, 2, ...}.
	iota(tempOrder.begin(), tempOrder.end(), 0);

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

	// If the label of the instruction is a target label for a branch, then we mark it as true. Otherwise, false.
	map<int, bool> isBranched;
	for (int i = 0; i < tempInstructions.size(); ++i) {
		if (tempInstructions[i][1] >= 6) {
			isBranched[tempInstructions[i][4]] = true;
		}
	}
	
	for (int i = 0; i < instructionsNumber - 1; ++i) {
		int previousInstruction = tempOrder[i], curInstruction = tempOrder[i + 1];
		if (instructions[previousInstruction][1] == 4 and instructions[curInstruction][1] < 6 and isBranched[oldNumber[curInstruction] - 1] == 0) {
			if (destination[previousInstruction] == sources[curInstruction][0] or destination[previousInstruction] == sources[curInstruction][1]) {
				for (int j = i + 2; j < instructionsNumber; ++j) {
					int candidateInstruction = tempOrder[j];

					// Don't cross branch.
					if (instructions[candidateInstruction][1] >= 6) {
						break;
					}

					// Don't cross a label that is connected to a branch.
					if (instructions[candidateInstruction][0] != -1 and isBranched.count(oldNumber[candidateInstruction] - 1) == 1) {
						break;
					}

					// Make sure that the candidate instruction doesn't read the previous instruction destination.
					if (destination[previousInstruction] != -1) {
						if (destination[previousInstruction] == sources[candidateInstruction][0] or destination[previousInstruction] == sources[candidateInstruction][1]) {
							continue;
						}
					}

					// Make sure that the candidate instruction doesn't write the previous instruction destination.
					if (destination[previousInstruction] != -1) {
						if (destination[candidateInstruction] == destination[previousInstruction]) {
							continue;
						}
					}

					// The instructions that candidate instruction depends on should stay above it (RAW).
					bool RAW = false;
					for (int k = j - 1; k >= i + 1; --k) {
						int maybeDependantInstruction = tempOrder[k];

						if (destination[maybeDependantInstruction] != -1) {
							if (destination[maybeDependantInstruction] == sources[candidateInstruction][0] or destination[maybeDependantInstruction] == sources[candidateInstruction][1]) {
								RAW = true;
								break;
							}
						}
					}

					if (RAW) {
						continue;
					}

					// The instructions that above the candidate instruction and have sources equals to the candidate instruction destination should stay above it (WAR).
					bool WAR = false;
					for (int k = j - 1; k >= i + 1; --k) {
						int maybeDependantInstruction = tempOrder[k];

						if (destination[candidateInstruction] != -1) {
							if (destination[candidateInstruction] == sources[maybeDependantInstruction][0] or destination[candidateInstruction] == sources[maybeDependantInstruction][1]) {
								WAR = true;
								break;
							}
						}
					}

					if (WAR) {
						continue;
					}

					// The instructions that above the candidate instruction and have destination equalt to the candidate instruction destination should stay above it (WAW).
					bool WAW = false;
					for (int k = j - 1; k >= i + 1; --k) {
						int maybeDependantInstruction = tempOrder[k];

						if (destination[maybeDependantInstruction] != -1) {
							if (destination[maybeDependantInstruction] == destination[candidateInstruction]) {
								WAW = true;
								break;
							}
						}
					}

					if (!WAW) {
						for (int k = j; k >= i + 2; --k) {
							swap(tempOrder[k], tempOrder[k - 1]);
						}

						break;
					}
				}
			}
		}
	}

	for (int i = 0; i < instructionsNumber; ++i) {
		int curInstruction = tempOrder[i];
		forwarding_reordering.push_back({});
		order2.push_back(oldNumber[curInstruction]);

		if (i) {
			int previousInstruction = tempOrder[i - 1];
			if ((instructions[previousInstruction][1] == 4 and (sources[curInstruction][0] == destination[previousInstruction] or sources[curInstruction][1] == destination[previousInstruction])) or (instructions[previousInstruction][1] >= 6 and (isAfter[oldNumber[previousInstruction] - 1] == true or isTaken[oldNumber[previousInstruction] - 1] == false))) {
				int j = 0;
				while (true) {
					if (forwarding_reordering[forwarding_reordering.size() - 2][j] == 0) {
						j++;
						break;
					}

					else {
						if (forwarding_reordering[forwarding_reordering.size() - 2][j] == -1) {
							forwarding_reordering.back().push_back(-1);
						}

						else {
							forwarding_reordering.back().push_back(-2);
						}

						j++;
					}
				}

				forwarding_reordering.back().push_back(-2);

				for (int k = 0; k < 1; ++k) {
					if (forwarding_reordering[forwarding_reordering.size() - 2].size() <= j) {
						forwarding_reordering.back().push_back(k);
						j++;
					}

					else {
						if (forwarding_reordering[forwarding_reordering.size() - 2][j] == -1) {
							forwarding_reordering.back().push_back(-1);
							j++;
							k--;
						}

						else {
							forwarding_reordering.back().push_back(k);
							j++;
						}
					}
				}

				while (forwarding_reordering[forwarding_reordering.size() - 2].size() > j and forwarding_reordering[forwarding_reordering.size() - 2][j] == -1) {
					forwarding_reordering.back().push_back(-1);
					j++;
				}

				forwarding_reordering.back().push_back(-1);
				j++;

				for (int k = 1; k < 5; ++k) {
					if (forwarding_reordering[forwarding_reordering.size() - 2].size() <= j) {
						forwarding_reordering.back().push_back(k);
						j++;
					}

					else {
						if (forwarding_reordering[forwarding_reordering.size() - 2][j] == -1) {
							forwarding_reordering.back().push_back(-1);
							j++;
							k--;
						}

						else {
							forwarding_reordering.back().push_back(k);
							j++;
						}
					}
				}
			}

			else {
				int j = 0;
				while (true) {
					if (forwarding_reordering[forwarding_reordering.size() - 2][j] == 0) {
						j++;
						break;
					}

					else {
						if (forwarding_reordering[forwarding_reordering.size() - 2][j] == -1) {
							forwarding_reordering.back().push_back(-1);
						}

						else {
							forwarding_reordering.back().push_back(-2);
						}
					}

					j++;
				}

				forwarding_reordering.back().push_back(-2);

				for (int k = 0; k < 5; ++k) {
					if (forwarding_reordering[forwarding_reordering.size() - 2].size() <= j) {
						forwarding_reordering.back().push_back(k);
						j++;
					}

					else {
						if (forwarding_reordering[forwarding_reordering.size() - 2][j] == -1) {
							forwarding_reordering.back().push_back(-1);
							j++;
							k--;
						}

						else {
							forwarding_reordering.back().push_back(k);
							j++;
						}
					}
				}
			}
		}

		else {
			for (int j = 0; j < 5; ++j) {
				forwarding_reordering.back().push_back(j);
			}
		}

		if (instructions[curInstruction][1] >= 6) {
			if (!isAfter[oldNumber[curInstruction] - 1]) {
				if (isTaken[oldNumber[curInstruction] - 1]) {
					forwarding_reordering.push_back({});
					order2.push_back(oldNumber[curInstruction] + 1);

					int curPointer = 0;
					while (forwarding_reordering[forwarding_reordering.size() - 2][curPointer] != 0) {
						forwarding_reordering.back().push_back(forwarding_reordering[forwarding_reordering.size() - 2][curPointer]);
						curPointer++;
					}

					forwarding_reordering.back().push_back(-2);
					curPointer++;

					while (forwarding_reordering[forwarding_reordering.size() - 2][curPointer] == -1) {
						forwarding_reordering.back().push_back(-1);
						curPointer++;
					}

					forwarding_reordering.back().push_back(0);
					forwarding_reordering.back().push_back(-1);
				}
			}
		}
	}

	int mx_cycles = 0;
	for (int i = 0; i < forwarding_reordering.size(); ++i) {
		mx_cycles = max(mx_cycles, (int)forwarding_reordering[i].size());
	}

	for (int i = 0; i < forwarding_reordering.size(); ++i) {
		while (forwarding_reordering[i].size() < mx_cycles) {
			forwarding_reordering[i].push_back(-2);
		}
	}

	for (int i = 0; i < mx_cycles; ++i) {
		for (int j = forwarding_reordering.size() - 1; j; --j) {
			if (forwarding_reordering[j - 1][i] == -1) {
				forwarding_reordering[j][i] = -2;
			}

			else {
				break;
			}
		}
	}
}

#endif