// you can use includes, for example:
// #include <algorithm>
#include <sstream>
// you can write to stdout for debugging purposes, e.g.
// cout << "this is a debug message" << endl;

void slice(int &slice_begin, int &slice_end, int new_begin, int new_end) {
	if (new_begin < slice_begin) {
		slice_begin = new_begin;
		slice_end = new_end;
	}
}

string solution(vector<int> &A) {
	vector<int> odd_positions;
	int number_of_even_before[A.size() + 1], current_begin = A.size(), current_end;
	number_of_even_before[0] = 0;

	for (int i = 0; i < A.size(); ++i) {
		A[i] %= 2;
		// count the number of even elements till this point
		number_of_even_before[i + 1] = number_of_even_before[i];
		if (A[i] == 0) {
			++number_of_even_before[i + 1];
		}
		else {
			// store the position of odd elements
			odd_positions.push_back(i);
		}
	}

	// for case of [even slice], where all odd elements are paired
	if (odd_positions.size() % 2 == 0) slice(current_begin, current_end, 0, A.size() - 1);

	// else if there is a stray odd element
	else {
		if (A.size() > 1) {
			// [even slice][odd element]
			if (A[A.size() - 1] == 1) slice(current_begin, current_end, 0, A.size() - 2);

			// [odd element][even slice]
			else if (A[0] == 1) slice(current_begin, current_end, 1, A.size() - 1);
		}

		if (A.size() > 3) {
			int left_even_elements, right_even_elements, slice_begin, slice_end;
			// if there's only 1 odd element 
			// (and the number of even elements on both sides are not the same)
			// ...[even element][odd element][even element]...
			if (odd_positions.size() == 1) {
				right_even_elements = number_of_even_before[A.size()] - number_of_even_before[odd_positions[0]];
				left_even_elements = number_of_even_before[odd_positions[0]];
				// if there are more even elements on left than on right
				if (left_even_elements > right_even_elements)
					slice(current_begin, current_end, 0, left_even_elements - right_even_elements - 1);

				// if there are more even elements on right than on left
				else if (left_even_elements < right_even_elements)
					slice(current_begin, current_end, odd_positions[0] + 1, odd_positions[0] + (right_even_elements - left_even_elements));
			}

			// if there are 2n+1 odd elements
			else {
				// check around last odd element
				// (all elements on right are even)
				right_even_elements = number_of_even_before[A.size()] - number_of_even_before[odd_positions[odd_positions.size() - 1]];
				left_even_elements = number_of_even_before[odd_positions[odd_positions.size() - 1]];
				slice_begin = odd_positions[0];
				slice_end = odd_positions[odd_positions.size() - 2];
				left_even_elements = (left_even_elements - right_even_elements) - (number_of_even_before[slice_end] - number_of_even_before[slice_begin]);

				if (left_even_elements >= 0) {
					while (slice_begin > 0 && left_even_elements > 0) {
						--slice_begin;
						--left_even_elements;
					}
					while (left_even_elements > 0) {
						++slice_end;
						--left_even_elements;
					}
					slice(current_begin, current_end, slice_begin, slice_end);
				}

				// check around first odd element
				// (all elements on left are even)
				right_even_elements = number_of_even_before[A.size()] - number_of_even_before[odd_positions[0]];
				left_even_elements = number_of_even_before[odd_positions[0]];
				slice_begin = odd_positions[1];
				slice_end = odd_positions[odd_positions.size() - 1];
				right_even_elements = (right_even_elements - left_even_elements) - (number_of_even_before[slice_end] - number_of_even_before[slice_begin]);

				if (right_even_elements >= 0) {
					while (slice_begin > odd_positions[0] + 1 && right_even_elements > 0) {
						--slice_begin;
						--right_even_elements;
					}
					while (right_even_elements > 0) {
						++slice_end;
						--right_even_elements;
					}
					slice(current_begin, current_end, slice_begin, slice_end);
				}
			}
		}
	}

	if (current_begin == A.size()) return "NO SOLUTION";
	else {
		ostringstream buffer;

		buffer << current_begin << "," << current_end;
		return buffer.str();
	}
}
