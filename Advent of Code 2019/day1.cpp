#include <vector>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <mutex>
#include <execution>
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <unordered_map>
#include "day1.h"

using namespace boost::multiprecision;
using namespace boost::algorithm;
using bigint_t = int256_t;

std::unordered_map<bigint_t, bigint_t> memo;

template <typename T>
constexpr auto fuelNeeded(T mass) {
	return mass / 3 - 2;
}

template <typename T>
constexpr T fuelNeededExtended(T mass) {
	if (auto val = memo[mass]; val != 0) {
		return val;
	}
	else if (mass > 0) {
		auto newMass = mass + fuelNeededExtended(fuelNeeded(mass));
		memo[mass] = newMass;
		return newMass;
	}
	else {
		return 0;
	}
}

void day1() {
	const auto fileName = "bigboyinput_day1.txt";
	std::ifstream in(fileName);
	const auto fileSize = std::filesystem::file_size(fileName);
	auto buf = new char[fileSize];
	in.read(buf, fileSize);

	auto input = std::string(buf);

	std::vector<std::string> lines;
	split(lines, input, is_any_of("\n"));

	std::vector<bigint_t> modules;

	for (auto& line : lines) {
		modules.emplace_back(bigint_t(line));
	}

	std::mutex sum_mut;
	bigint_t sum = 0;

	std::for_each(std::execution::par, modules.begin(), modules.end(), [&](auto& m) {
		auto fuel = fuelNeededExtended(fuelNeeded(m));
		sum_mut.lock();
		sum += fuel;
		sum_mut.unlock();
		});

	std::cout << sum << '\n';
}