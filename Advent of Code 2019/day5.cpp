#include <deque>
#include <functional>
#include <iostream>
#include <map>
#include <optional>
#include <vector>
#include <fstream>
#include <filesystem>
#include <mutex>
#include <thread>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/multiprecision/cpp_int.hpp>
#include "intcomputer.h"

using namespace boost::algorithm;
void day7() {
	auto fileName = "input_day7.txt";
	std::ifstream file(fileName);
	const auto fileSize = std::filesystem::file_size(fileName);
	auto buf = new char[fileSize];
	file.read(buf, fileSize);

	auto input = std::string(buf);

	std::vector<std::string> words;
	split(words, input, is_any_of(","));

	std::vector<int> instructions(words.size());
	for (size_t i = 0; i < words.size(); i++) {
		instructions[i] = std::stoi(words[i]);
	}

	auto maxSignalWithFeedback = [&instructions](std::vector<int> phases) -> int {
		int computerInput = 0;

		auto first = intComputer(instructions, phases[0]);
		computerInput = first.compute(computerInput).first;
		auto second = intComputer(instructions, phases[1]);
		computerInput = second.compute(computerInput).first;
		auto third = intComputer(instructions, phases[2]);
		computerInput = third.compute(computerInput).first;
		auto fourth = intComputer(instructions, phases[3]);
		computerInput = fourth.compute(computerInput).first;
		auto fifth = intComputer(instructions, phases[4]);
		computerInput = fifth.compute(computerInput).first;

		while (true) {
			{
				auto [val, done] = first.compute(computerInput);
				computerInput = val;
			}
			{
				auto [val, done] = second.compute(computerInput);
				computerInput = val;
			}
			{
				auto [val, done] = third.compute(computerInput);
				computerInput = val;
			}
			{
				auto [val, done] = fourth.compute(computerInput);
				computerInput = val;
			}
			{
				auto [val, done] = fifth.compute(computerInput);
				if (done)
					return val;
				else computerInput = val;
			}

		}
		return computerInput;
	};

	auto amplifier_settings = std::vector{ 5,6,7,8,9 };
	auto vals = std::vector<int>();
	do {
		vals.emplace_back(maxSignalWithFeedback(amplifier_settings));
	} while (std::next_permutation(amplifier_settings.begin(), amplifier_settings.end()));

	std::cout << *std::max_element(vals.begin(),vals.end()) << '\n';
}