#include <vector>
#include <deque>
#include <map>
#include <optional>
#include <stdexcept>

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
	adjustBase = 9,
};

enum class parameterMode {
	position = 0,
	immediate = 1,
	relative = 2,
};

template <typename T>
class intComputer {
private:
	std::vector<T> instructions;
	T lastOutput = 0;

	typename std::vector<T>::iterator storedIP;
	typename std::vector<T>::iterator storedRBP;
	std::deque<T> temp_input;

	std::map<opCodeID, size_t> const length{
		{opCodeID::halt, 0},      {opCodeID::plus, 3},
		{opCodeID::multiply, 3},  {opCodeID::store, 1},
		{opCodeID::output, 1},    {opCodeID::jumpTrue, 2},
		{opCodeID::jumpFalse, 2}, {opCodeID::less, 3},
		{opCodeID::equals, 3},    {opCodeID::adjustBase, 1},
	};

	auto decodeOpcode(T op) -> std::pair<opCodeID, std::vector<parameterMode>> {
		auto opID = static_cast<opCodeID>(op % 100);  // reads bottom two digits
		auto size = length.at(opID);

		op /= 100;  // Remove bottom two digits
		std::vector<parameterMode> modeFlags;
		for (size_t i = 0; i < size; i++) {
			modeFlags.emplace_back(static_cast<parameterMode>(op % 10));
			op /= 10;
		}

		return { opID, modeFlags };
	}
	auto read() -> T {
		auto val = temp_input.front();
		temp_input.pop_front();
		return val;
	}

	auto apply(opCodeID op, std::vector<parameterMode> mode,
		std::vector<T> param) -> std::pair<std::optional<T>, std::optional<T>> {

		auto readArguments = [&mode, &param, this](size_t n) -> std::vector<T> {
			std::vector<T> args(n);

			for (size_t i = 0; i < args.size(); i++) {
				if (mode[i] == parameterMode::immediate)
					args[i] = param[i];
				else if (mode[i] == parameterMode::position)
					args[i] = instructions[size_t(param[i])];
				else if (mode[i] == parameterMode::relative)
					args[i] = *(storedRBP + param[i]);
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

			if (mode.back() == parameterMode::immediate)
				throw std::domain_error("Illegal write mode");
			if (mode.back() == parameterMode::position)
				instructions[param.back()] = fn(args[0], args[1]);
			if (mode.back() == parameterMode::relative)
				*(storedRBP + param.back()) = fn(args[0], args[1]);
			break;
		}
		case opCodeID::less: {
			auto fn = std::less<>();
			auto args = readArguments(length.at(op) - 1);

			if (mode.back() == parameterMode::immediate)
				throw std::domain_error("Illegal write mode");
			if (mode.back() == parameterMode::position)
				if (fn(args[0], args[1]))
					instructions[param.back()] = 1;
				else
					instructions[param.back()] = 0;
			else if (mode.back() == parameterMode::relative)
				if (fn(args[0], args[1]))
					*(storedRBP + param.back()) = 1;
				else
					*(storedRBP + param.back()) = 0;
			break;
		}
		case opCodeID::equals: {
			auto fn = std::equal_to<>();
			auto args = readArguments(length.at(op) - 1);

			if (mode.back() == parameterMode::immediate)
				throw std::domain_error("Illegal write mode");
			if (mode.back() == parameterMode::position)
				if (fn(args[0], args[1]))
					instructions[param.back()] = 1;
				else
					instructions[param.back()] = 0;
			else if (mode.back() == parameterMode::relative)
				if (fn(args[0], args[1]))
					*(storedRBP + param.back()) = 1;
				else
					*(storedRBP + param.back()) = 0;
			break;
		}

		case opCodeID::multiply:
			// Just a copy paste from plus but with a different bind argument,
			// when there is a clear pattern this should be factored out.
		{
			auto fn = std::multiplies<>();
			auto args = readArguments(length.at(op) - 1);

			if (mode.back() == parameterMode::immediate)
				throw std::domain_error("Illegal write mode");
			if (mode.back() == parameterMode::position)
				instructions[param.back()] = fn(args[0], args[1]);
			else if (mode.back() == parameterMode::relative)
				*(storedRBP + param.back()) = fn(args[0], args[1]);
			break;
		}

		case opCodeID::jumpFalse: {
			auto args = readArguments(length.at(op));

			if (args[0] == 0) return { args[1],std::nullopt };
			break;
		}

		case opCodeID::jumpTrue: {
			auto args = readArguments(length.at(op));

			if (args[0] != 0) return { args[1],std::nullopt };
			break;
		}

		case opCodeID::store: {
			if (mode[0] == parameterMode::immediate)
				throw std::domain_error("Illegal write mode");
			else if (mode[0] == parameterMode::position)
				instructions[param[0]] = read();
			else if (mode[0] == parameterMode::relative)
				*(storedRBP + param[0]) = read();
			break;
		}
		case opCodeID::output: {
			//T value = 0;
			//if (mode[0] == parameterMode::immediate)
			//	value = param[0];
			//else if (mode[0] == parameterMode::position)
			//	value = instructions[size_t(param[0])];
			auto value = readArguments(length.at(op))[0];
			return { std::nullopt,value };
		}
		case opCodeID::adjustBase: {
			auto args = readArguments(length.at(op));
			storedRBP += args[0];
			break;
		}
		default:
			throw std::domain_error("Exhausted the switch!");
		};
		return { std::nullopt,std::nullopt };
	}

public:
	auto addInput(T in) {
		temp_input.emplace_back(in);
	}
	std::pair<T, bool> compute() {
		for (auto current = storedIP; current < instructions.end();
			) {
			auto [op, flags] = decodeOpcode(*current);
			std::vector<T> params(flags.size());
			for (auto i = 0; i < params.size(); i++) {
				current++;
				params[i] = *current;
			}
			if (op == opCodeID::halt) {
				return { temp_input.front(), true };
			}

			if (auto [offset, ret] = apply(op, flags, params); offset != std::nullopt)
				current = instructions.begin() + size_t(offset.value());
			else if (ret != std::nullopt) {
				current++;
				storedIP = current;
				return std::pair<T, bool>{ ret.value(), false };
			}
			else
				current++;
			// for (auto i = instructions.begin(); i < instructions.end(); i++)
			//  std::cout << *i << '\n';
		}
		// for (auto i = output.begin(); i < output.end(); i++) std::cout << *i <<
		// '\n';
		return { lastOutput, false };
	}

	intComputer(std::vector<T> code, T phase) :
		instructions(code), storedIP(instructions.begin()), storedRBP(instructions.begin()), temp_input{ phase } {}
	intComputer(std::vector<T> code) :
		instructions(code), storedIP(instructions.begin()), storedRBP(instructions.begin()) {}
};