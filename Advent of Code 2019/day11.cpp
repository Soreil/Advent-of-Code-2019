#include "day11.h"
#include "intcomputer.h"
#include <fstream>
#include <filesystem>
#include <set>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/multiprecision/cpp_int.hpp>

using namespace boost::algorithm;


enum class dir {
	up,
	right,
	down,
	left,
};

auto left(dir d) {
	if (d == dir::up)
		return dir::left;
	return dir(static_cast<int>(d) - 1);
}
auto right(dir d) {
	if (d == dir::left)
		return dir::up;
	return dir(static_cast<int>(d) + 1);
}
using point = std::pair<int, int>;

auto move(point p, dir d) {
	switch (d) {
	case dir::up:
		return point{ p.first,p.second - 1 };
	case dir::right:
		return point{ p.first + 1,p.second };
	case dir::down:
		return point{ p.first,p.second + 1 };
	case dir::left:
		return point{ p.first - 1,p.second };
	default:
		throw std::domain_error("Illegal direction");
	}
}

void day11() {

	auto fileName = "input_day11.txt";
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


	std::set<point> visited;
	std::map<point, size_t> hitCount;
	std::map<point, bool> isWhite;
	dir heading = dir::up;
	point position{ 0,0 };
	isWhite[position] = true;

	while (true) {
		computer.addInput(int64_t(isWhite[position]));

		auto [colour, stop] = computer.compute();
		visited.insert(position);
		hitCount[position]++;
		if (colour == 1) isWhite[position] = true;
		else if (colour == 0) isWhite[position] = false;
		else throw std::domain_error("Unknown colour");
		if (stop) break;

		auto [direction, impossible] = computer.compute();
		if (impossible) throw std::domain_error("Can't stop here");
		if (direction == 1) heading = right(heading);
		else if (direction == 0) heading = left(heading);
		else throw std::domain_error("Unknown direction");

		position = move(position, heading);

		//std::cout << "colour:" << colour << " direction:" << direction << '\n';
	}

	int minX = 0;
	int minY = 0;
	int maxX = 0;
	int maxY = 0;

	for (auto p : visited) {
		if (p.first < minX)
			minX = p.first;
		if (p.first > maxX)
			maxX = p.first;
		if (p.second < minY)
			minY = p.first;
		if (p.second > maxY)
			maxY = p.first;
	}

	for (int y = minY; y <= maxY; y++) {
		for (int x = minX; x <= maxX; x++) {
			char c = ' ';
			if (isWhite[point{ x,y }])
				c = '#';
			std::cout << c;
		}
		std::cout << '\n';
	}
}

