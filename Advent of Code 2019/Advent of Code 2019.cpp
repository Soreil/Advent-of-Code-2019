#include <iostream>
#include <vector>
#include <chrono>
#include "day1.h"
#include "day2.h"
#include "day3.h"

using namespace std::chrono;

int main() {
	auto begin = high_resolution_clock::now();

	day3();

	auto end = high_resolution_clock::now();
	auto duration = duration_cast<milliseconds>(end - begin);
	std::cout << duration.count() << '\n';
}