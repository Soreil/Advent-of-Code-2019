#include <iostream>
#include <vector>
#include <chrono>
#include "day12.h"

using namespace std::chrono;

int main() {
	auto begin = high_resolution_clock::now();

	day12();

	auto end = high_resolution_clock::now();
	auto duration = duration_cast<milliseconds>(end - begin);
	std::cout << "duration:"<< duration.count() << '\n';
}