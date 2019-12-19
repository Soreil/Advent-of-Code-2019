#include <fstream>
#include <filesystem>
#include <iostream>
#include <boost/algorithm/string.hpp>
#include <conio.h>
#include "day13.h"
#include "intcomputer.h"
using namespace boost::algorithm;

using point = std::pair<size_t, size_t>;

void day13() {

	auto fileName = "input_day13.txt";
	std::ifstream file(fileName);
	const auto fileSize = std::filesystem::file_size(fileName);
	std::string input(fileSize, '\0');
	file.read(input.data(), fileSize);
	input.resize(file.tellg());

	std::vector<std::string> words;
	split(words, input, is_any_of(","));

	std::vector<int64_t> instructions(words.size() + 0xffff);
	for (size_t i = 0; i < words.size(); i++) {
		instructions[i] = std::stoll(words[i]);
	}
	instructions[0] = 2; //Add quarters :)
	instructions.resize(instructions.size());

	auto computer = intComputer(instructions);

	std::map<point, int> tiles;

	computer.addInput(0); //Set to neutral joystick at the start

	enum class messageType {
		tile,
		score
	};

	auto getScore = [&]() -> std::optional<int> {
		std::vector<int> out(3);

		if (auto [ret, done] = computer.compute(); !done) {
			out[0] = ret;
		}
		else {
			return std::nullopt;
		}
		if (auto [ret, done] = computer.compute(); !done) {
			out[1] = ret;
		}
		else {
			return std::nullopt;
		}
		if (auto [ret, done] = computer.compute(); !done) {
			out[2] = ret;
		}
		else {
			return std::nullopt;
		}

		if (out[0] == -1 && out[1] == 0) {
			return out[2];
		}
		tiles[{out[0], out[1]}] = out[2];
		return -1;
	};

	for (size_t i = 0; i < 1092; i++) {
		getScore();
	}

	auto min = (*tiles.begin()).first;
	auto max = (*std::next(tiles.begin(), tiles.size() - 1)).first;

	auto score = getScore().value();

	for (int turn = 0; turn < 0xffff; turn++) {
		//std::cout << score << '\n';
		for (int i = 0; i < 2; i++) {
			auto val = getScore();
			if (!val.has_value()) {
				return;
			}
			if (val.value() >= 0) {
				score = val.value();
			}
		}
		//system("cls");

		point paddle;
		point ball;
		for (int y = min.second; y < max.second; y++) {
			for (int x = min.first; x < max.first; x++) {
				auto tile = tiles[{x, y}];
				char c;
				switch (tile) {
				case 0:
					c = ' ';
					break;
				case 1:
					c = '#';
					break;
				case 2:
					c = '+';
					break;
				case 3:
					paddle = { x,y };
					c = '-';
					break;
				case 4:
					ball = { x,y };
					c = '0';
					break;
				default:
					throw std::domain_error("illegal tile");
				}
				//std::cout << c;
			}
			//std::cout << '\n';
		}
		std::cout << "Final score:" << score << " ball:" << ball.first << ',' << ball.second << " bat:" << paddle.first << ',' << paddle.second << '\n';

		//char cmd;
		//cmd = _getch();
		int direction = 0;
		if (paddle.first < ball.first)
			direction = 1;
		if (paddle.first == ball.first)
			direction = 0;
		if (paddle.first > ball.first)
			direction = -1;
		//switch (cmd) {
		//case 'h':
		//	direction = -1;
		//	break;
		//case 'j':
		//	direction = 0;
		//	break;
		//case 'k':
		//	direction = 1;
		//	break;
		//default:
		//	throw std::domain_error("Unknown keycode");
		//}

		computer.read = [direction]() {
			return direction;
		};
	}
}