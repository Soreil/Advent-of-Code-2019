#include <algorithm>
#include <iostream>
#include <string>

int main() {
	auto const lower = 134792;
	auto const upper = 675810;

	auto isValid = [](auto const n) -> bool {
		auto const s = std::to_string(n);
		if (s.size() != 6) return false;
		if (!std::is_sorted(s.begin(), s.end())) return false;

		auto previous = s.begin();
		while (true) {
			if (auto adjacent = std::adjacent_find(previous, s.end());
				adjacent != s.end()) {
				if (std::count(std::prev(adjacent), s.end(), *adjacent) == 2)
					return true;
				previous = std::next(adjacent);
			}
			else
				return false;
		}
	};

	size_t validPasswords = 0;
	for (auto current = lower; current < upper; current++) {
		if (isValid(current)) validPasswords++;
	}

	std::cout << validPasswords << '\n';
}