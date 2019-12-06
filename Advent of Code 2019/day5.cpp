#include <deque>
#include <functional>
#include <iostream>
#include <map>
#include <optional>
#include <vector>
#include <fstream>
#include <filesystem>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/multiprecision/cpp_int.hpp>

using namespace boost::algorithm;

enum class opCodeID {
	halt = 99,
	plus = 1,
	multiply = 2,
	store = 3,
	output = 4,
	jumpTrue = 5,
	jumpFalse = 6,
	less = 7,
	equals = 8,
	outputScreen = 9,
};

enum class parameterMode {
	position = 0,
	immediate = 1,
};

class intComputer {
private:
	std::deque<int> inputBuffer{ 1 };
	std::deque<int> output;
	std::vector<int> instructions;

	std::map<opCodeID, size_t> const length{
		{opCodeID::halt, 0},      {opCodeID::plus, 3},
		{opCodeID::multiply, 3},  {opCodeID::store, 1},
		{opCodeID::output, 1},    {opCodeID::jumpTrue, 2},
		{opCodeID::jumpFalse, 2}, {opCodeID::less, 3},
		{opCodeID::equals, 3},    {opCodeID::outputScreen, 1},
	};

	template <typename T>
	auto decodeOpcode(T op) -> std::pair<opCodeID, std::vector<parameterMode>> {
		auto opID = static_cast<opCodeID>(op % 100);  // reads bottom two digits
		auto size = length.at(opID);

		op /= 100;  // Remove bottom two digits
		std::vector<parameterMode> modeFlags;
		for (size_t i = 0; i < size; i++) {
			modeFlags.emplace_back(static_cast<parameterMode>(op % 10 == 1));
			op /= 10;
		}

		return { opID, modeFlags };
	}

	auto read() -> int {
		auto out = inputBuffer.front();
		inputBuffer.pop_front();
		return out;
	}

	auto write(int val) -> void { output.emplace_back(val); }

	auto apply(opCodeID op, std::vector<parameterMode> mode,
		std::vector<int> param) -> std::optional<int> {
		using namespace std::placeholders;

		auto readArguments = [&mode, &param, this](size_t n) -> std::vector<int> {
			std::vector<int> args(n);

			for (size_t i = 0; i < args.size(); i++) {
				if (mode[i] == parameterMode::immediate)
					args[i] = param[i];
				else if (mode[i] == parameterMode::position)
					args[i] = instructions[size_t(param[i])];
				else
					throw std::domain_error("Unimplemented parameter mode");
			}
			return args;
		};

		switch (op) {
		case opCodeID::halt: {
			throw std::domain_error("We shouldn't have entered here with a halt");
		}
		case opCodeID::plus: {
			auto fn = std::plus<>();
			auto args = readArguments(length.at(op) - 1);

			if (mode.back() != parameterMode::position)
				throw std::domain_error("Illegal write mode");
			instructions[param.back()] = fn(args[0], args[1]);
			break;
		}
		case opCodeID::less: {
			auto fn = std::less<>();
			auto args = readArguments(length.at(op) - 1);

			if (mode.back() != parameterMode::position)
				throw std::domain_error("Illegal write mode");
			if (fn(args[0], args[1]))
				instructions[param.back()] = 1;
			else
				instructions[param.back()] = 0;

			break;
		}
		case opCodeID::equals: {
			auto fn = std::equal_to<>();
			auto args = readArguments(length.at(op) - 1);

			if (mode.back() != parameterMode::position)
				throw std::domain_error("Illegal write mode");
			if (fn(args[0], args[1]))
				instructions[param.back()] = 1;
			else
				instructions[param.back()] = 0;
			break;
		}

		case opCodeID::multiply:
			// Just a copy paste from plus but with a different bind argument,
			// when there is a clear pattern this should be factored out.
		{
			auto fn = std::multiplies<>();
			auto args = readArguments(length.at(op) - 1);

			if (mode.back() != parameterMode::position)
				throw std::domain_error("Illegal write mode");
			instructions[param.back()] = fn(args[0], args[1]);
			break;
		}

		case opCodeID::jumpFalse: {
			auto args = readArguments(length.at(op));

			if (args[0] == 0) return args[1];
			break;
		}

		case opCodeID::jumpTrue: {
			auto args = readArguments(length.at(op));

			if (args[0] != 0) return args[1];
			break;
		}

		case opCodeID::store: {
			if (mode[0] != parameterMode::position)
				throw std::domain_error("Illegal write mode");
			instructions[param[0]] = read();
			break;
		}
		case opCodeID::output: {
			int value = 0;
			if (mode[0] == parameterMode::immediate)
				value = param[0];
			else if (mode[0] == parameterMode::position)
				value = instructions[size_t(param[0])];

			write(value);
			break;
		}
		case opCodeID::outputScreen: {
			int value = 0;
			if (mode[0] == parameterMode::immediate)
				value = param[0];
			else if (mode[0] == parameterMode::position)
				value = instructions[size_t(param[0])];

			std::cout << (char(char(value)));
			break;
		}
		};
		return std::nullopt;
	}

public:
	int compute() {
		for (auto current = instructions.begin(); current < instructions.end();
			) {
			auto [op, flags] = decodeOpcode(*current);
			std::vector<int> params(flags.size());
			for (auto i = 0; i < params.size(); i++) {
				current++;
				params[i] = *current;
			}
			if (op == opCodeID::halt) break;
			if (auto offset = apply(op, flags, params); offset != std::nullopt)
				current = instructions.begin() + size_t(offset.value());
			else
					current++;
			// for (auto i = instructions.begin(); i < instructions.end(); i++)
			//  std::cout << *i << '\n';
		}
		// for (auto i = output.begin(); i < output.end(); i++) std::cout << *i <<
		// '\n';
		if (output.size() > 0)
			return output.back();
		else
			return 0;
	}
	intComputer(std::vector<int> code) :instructions(code) {}
};

void day5() {
	auto fileName = "input_ascii_day5.txt";
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

	intComputer comp(instructions);
	std::cout << comp.compute() << '\n';
}
