#include "intcomputer.h"
#include <fstream>
#include <filesystem>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/multiprecision/cpp_int.hpp>
#include "day9.h"

using namespace boost::algorithm;

void day9() {
	auto fileName = "input_day9.txt";
	std::ifstream file(fileName);
	const auto fileSize = std::filesystem::file_size(fileName);
	auto buf = new char[fileSize];
	file.read(buf, fileSize);

	auto input = std::string(buf);

	std::vector<std::string> words;
	split(words, input, is_any_of(","));

	std::vector<int64_t> instructions(words.size());
	for (size_t i = 0; i < words.size(); i++) {
		instructions[i] = std::stoll(words[i]);
	}

	instructions.resize(instructions.size() + 0xffff);

	auto computer = intComputer(instructions);
	while (true) {
		auto [ret,stop] = computer.compute(int64_t(2));
		if (stop)
			break;
		std::cout << ret  << '\n';
	}
}