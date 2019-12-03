#include <stdint.h>

#include <functional>
#include <iostream>
#include <vector>
#include <fstream>
#include <filesystem>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/multiprecision/cpp_int.hpp>
#include <execution>
#include <algorithm>
#include <numeric>

using namespace boost::algorithm;
using namespace boost::multiprecision;

using bigint_t = int128_t;

template< typename T>
void executeProgram(std::vector<T>& input) {
	for (size_t instruction_pointer = 0; instruction_pointer < (input.size() - 4);
		instruction_pointer += 4) {
		auto opcode = input[instruction_pointer];
		auto lhs = input[instruction_pointer + 1];
		auto rhs = input[instruction_pointer + 2];
		auto dest = input[instruction_pointer + 3];

		std::function<T(T, T)> op;

		if (opcode == 1) {
			op = std::plus<T>();
		}
		else if (opcode == 2) {
			op = std::multiplies<T>();
		}
		else if (opcode == 99) {
			return;
		}

		input[size_t(dest)] = op(input[size_t(lhs)], input[size_t(rhs)]);

	}
}

void day2() {
	auto fileName = "D2P2-RealisticBigBoy.txt";
	std::ifstream file(fileName);
	const auto fileSize = std::filesystem::file_size(fileName);
	auto buf = new char[fileSize];
	file.read(buf, fileSize);

	auto input = std::string(buf);

	std::vector<std::string> words;
	split(words, input, is_any_of(","));

	std::vector<bigint_t> instructions(words.size());
	for (size_t i = 0; i < words.size(); i++) {
		instructions[i] = bigint_t(words[i]);
	}

	std::vector<size_t> noun(100);
	std::iota(noun.begin(), noun.end(), 0);
	std::for_each(std::execution::par_unseq, noun.begin(), noun.end(), [&](size_t noun) {

		std::vector<size_t> verb(100);
		std::iota(verb.begin(), verb.end(), 0);
		std::for_each(std::execution::par_unseq, verb.begin(), verb.end(), [&](size_t verb) {
			auto input = instructions;
			input[1] = noun;
			input[2] = verb;
			executeProgram(input);
			if (input[0] == 19690720) {
				std::cout << noun << ' ' << verb << ' ' << input[0] << '\n';
				std::cout << 100 * noun + verb << '\n';
			}
			});
		});
}