#include <deque>
#include <functional>
#include <iostream>
#include <map>
#include <optional>
#include <vector>

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
};

enum class parameterMode {
	position = 0,
	immediate = 1,
};

class intComputer {
private:
	std::deque<int> inputBuffer{ 5 };
	std::deque<int> output;

	std::vector<int> instructions{
		3,     225,   1,     225,   6,     6,     1100, 1,     238,  225,   104,
		0,     1101,  81,    30,    225,   1102,  9,    63,    225,  1001,  92,
		45,    224,   101,   -83,   224,   224,   4,    224,   102,  8,     223,
		223,   101,   2,     224,   224,   1,     224,  223,   223,  1102,  41,
		38,    225,   1002,  165,   73,    224,   101,  -2920, 224,  224,   4,
		224,   102,   8,     223,   223,   101,   4,    224,   224,  1,     223,
		224,   223,   1101,  18,    14,    224,   1001, 224,   -32,  224,   4,
		224,   1002,  223,   8,     223,   101,   3,    224,   224,  1,     224,
		223,   223,   1101,  67,    38,    225,   1102, 54,    62,   224,   1001,
		224,   -3348, 224,   4,     224,   1002,  223,  8,     223,  1001,  224,
		1,     224,   1,     224,   223,   223,   1,    161,   169,  224,   101,
		-62,   224,   224,   4,     224,   1002,  223,  8,     223,  101,   1,
		224,   224,   1,     223,   224,   223,   2,    14,    18,   224,   1001,
		224,   -1890, 224,   4,     224,   1002,  223,  8,     223,  101,   3,
		224,   224,   1,     223,   224,   223,   1101, 20,    25,   225,   1102,
		40,    11,    225,   1102,  42,    58,    225,  101,   76,   217,   224,
		101,   -153,  224,   224,   4,     224,   102,  8,     223,  223,   1001,
		224,   5,     224,   1,     224,   223,   223,  102,   11,   43,    224,
		1001,  224,   -451,  224,   4,     224,   1002, 223,   8,    223,   101,
		6,     224,   224,   1,     223,   224,   223,  1102,  77,   23,    225,
		4,     223,   99,    0,     0,     0,     677,  0,     0,    0,     0,
		0,     0,     0,     0,     0,     0,     0,    1105,  0,    99999, 1105,
		227,   247,   1105,  1,     99999, 1005,  227,  99999, 1005, 0,     256,
		1105,  1,     99999, 1106,  227,   99999, 1106, 0,     265,  1105,  1,
		99999, 1006,  0,     99999, 1006,  227,   274,  1105,  1,    99999, 1105,
		1,     280,   1105,  1,     99999, 1,     225,  225,   225,  1101,  294,
		0,     0,     105,   1,     0,     1105,  1,    99999, 1106, 0,     300,
		1105,  1,     99999, 1,     225,   225,   225,  1101,  314,  0,     0,
		106,   0,     0,     1105,  1,     99999, 8,    226,   677,  224,   1002,
		223,   2,     223,   1006,  224,   329,   1001, 223,   1,    223,   7,
		226,   226,   224,   102,   2,     223,   223,  1006,  224,  344,   101,
		1,     223,   223,   108,   677,   677,   224,  1002,  223,  2,     223,
		1006,  224,   359,   101,   1,     223,   223,  1107,  226,  677,   224,
		1002,  223,   2,     223,   1005,  224,   374,  101,   1,    223,   223,
		1008,  677,   226,   224,   1002,  223,   2,    223,   1005, 224,   389,
		101,   1,     223,   223,   1007,  677,   226,  224,   1002, 223,   2,
		223,   1005,  224,   404,   1001,  223,   1,    223,   1107, 677,   226,
		224,   1002,  223,   2,     223,   1005,  224,  419,   1001, 223,   1,
		223,   108,   677,   226,   224,   102,   2,    223,   223,  1006,  224,
		434,   1001,  223,   1,     223,   7,     226,  677,   224,  102,   2,
		223,   223,   1005,  224,   449,   1001,  223,  1,     223,  107,   226,
		226,   224,   102,   2,     223,   223,   1006, 224,   464,  101,   1,
		223,   223,   107,   677,   226,   224,   102,  2,     223,  223,   1006,
		224,   479,   101,   1,     223,   223,   1007, 677,   677,  224,   1002,
		223,   2,     223,   1006,  224,   494,   1001, 223,   1,    223,   1008,
		226,   226,   224,   1002,  223,   2,     223,  1006,  224,  509,   101,
		1,     223,   223,   7,     677,   226,   224,  1002,  223,  2,     223,
		1006,  224,   524,   1001,  223,   1,     223,  1007,  226,  226,   224,
		102,   2,     223,   223,   1006,  224,   539,  101,   1,    223,   223,
		8,     677,   226,   224,   1002,  223,   2,    223,   1006, 224,   554,
		101,   1,     223,   223,   1008,  677,   677,  224,   102,  2,     223,
		223,   1006,  224,   569,   101,   1,     223,  223,   1108, 677,   226,
		224,   102,   2,     223,   223,   1005,  224,  584,   101,  1,     223,
		223,   107,   677,   677,   224,   102,   2,    223,   223,  1006,  224,
		599,   1001,  223,   1,     223,   1108,  677,  677,   224,  1002,  223,
		2,     223,   1006,  224,   614,   1001,  223,  1,     223,  1107,  677,
		677,   224,   1002,  223,   2,     223,   1005, 224,   629,  1001,  223,
		1,     223,   108,   226,   226,   224,   1002, 223,   2,    223,   1005,
		224,   644,   101,   1,     223,   223,   8,    226,   226,  224,   1002,
		223,   2,     223,   1005,  224,   659,   101,  1,     223,  223,   1108,
		226,   677,   224,   1002,  223,   2,     223,  1006,  224,  674,   101,
		1,     223,   223,   4,     223,   99,    226 };

	std::map<opCodeID, size_t> const length{
		{opCodeID::halt, 0},      {opCodeID::plus, 3},   {opCodeID::multiply, 3},
		{opCodeID::store, 1},     {opCodeID::output, 1}, {opCodeID::jumpTrue, 2},
		{opCodeID::jumpFalse, 2}, {opCodeID::less, 3},   {opCodeID::equals, 3},
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

	auto apply(opCodeID op, std::vector<parameterMode> mode, std::vector<int> param)
		-> std::optional<int> {
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
		case opCodeID::halt:
		{
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

		case opCodeID::jumpFalse:
		{
			auto args = readArguments(length.at(op));

			if (args[0] == 0)
				return args[1];
			break;
		}

		case opCodeID::jumpTrue:
		{
			auto args = readArguments(length.at(op));

			if (args[0] != 0)
				return args[1];
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
		};
		return std::nullopt;
	}

	public:
	int compute() {
		for (auto current = instructions.begin(); current < instructions.end();
			current++) {
			auto [op, flags] = decodeOpcode(*current);
			std::vector<int> params(flags.size());
			for (auto i = 0; i < params.size(); i++) {
				current++;
				params[i] = *current;
			}
			if (op == opCodeID::halt)
				break;
			if (auto offset = apply(op, flags, params); offset != std::nullopt)
				current = instructions.begin() + size_t(offset.value() - 1);

			// for (auto i = instructions.begin(); i < instructions.end(); i++)
			//  std::cout << *i << '\n';
		}
		//for (auto i = output.begin(); i < output.end(); i++) std::cout << *i << '\n';
		return output.back();
	}
};

void day5() {
	intComputer comp;
	std:: cout << comp.compute() << '\n';
}
	