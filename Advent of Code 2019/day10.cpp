#define _USE_MATH_DEFINES
#include <cmath>
#include <algorithm>
#include <iostream>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>
#include <set>


class space {
	std::vector<std::vector<bool>> asteroidPresent;

public:
	size_t const width;
	size_t const height;

	space(std::vector<std::string> s)
		: asteroidPresent(s.size()), height(s.size()), width(s[0].size()) {
		std::for_each(s.begin(), s.end(), [i = 0, this](auto const& l) mutable {
			std::transform(l.begin(), l.end(),
				std::back_inserter(asteroidPresent[i++]),
				[](auto const& c) { return c == '.'; });
		});
	};

	auto hasAsteroid(int x, int y) -> bool { return !asteroidPresent[y][x]; }

	auto destroy(std::vector<std::pair<size_t, size_t>> target) {
		for (auto [x, y] : target) {
			asteroidPresent[y][x] = true;
		}
	}

	void show() {
		std::for_each(
			asteroidPresent.begin(), asteroidPresent.end(), [](auto const& l) {
				std::for_each(l.begin(), l.end(), [](auto c) { std::cout << c; });
				std::cout << '\n';
			});
		std::cout << width << ',' << height << '\n';
	};

	auto visible(size_t x, size_t y) -> std::vector<std::pair<size_t, size_t>> {
		size_t count = 0;

		std::vector<std::vector<bool>> visited(
			asteroidPresent.size(),
			std::vector<bool>(asteroidPresent[0].size()));  //= asteroidPresent;
		std::vector<std::pair<size_t, size_t>> targets;

		auto distance = [](int x1, int y1, int x2, int y2) -> std::pair<int, int> {
			return { std::abs(x1 - x2), std::abs(y1 - y2) };
		};

		auto mark = [&](size_t xPos, size_t yPos) -> bool {
			if (visited[yPos][xPos]) return false;

			auto d = distance(x, y, int(xPos), int(yPos));
			auto gcd = std::gcd(d.first, d.second);
			// std::cout << xPos << ',' << yPos << ',' << gcd << '\n';

			for (int i = 1; i <= gcd; i++) {
				auto offsetY = ((d.second / gcd) * i);
				auto offsetX = ((d.first / gcd) * i);
				if (xPos < x) offsetX = -offsetX;
				if (yPos < y) offsetY = -offsetY;

				if (!visited[y + offsetY][x + offsetX]) {
					if (!asteroidPresent[y + offsetY][x + offsetX]) {
						//std::cout << "Found asteroid at" << x + offsetX << ','
						//	<< y + offsetY << '\n';
						visited[y + offsetY][x + offsetX] = true;
						targets.emplace_back(std::pair{ x + offsetX,y + offsetY });
						return true;
					}
					//visited[y + offsetY][x + offsetX] = true;
				}
				else {
					break;
				}
			}
			return false;
		};

		// std::cout << "distances to:" << x << ',' << y << '\n';

		for (int ring = 0; ring < height; ring++) {
			size_t xPos = ring;
			for (size_t yPos = ring; yPos < height - ring; yPos++) {
				if (mark(xPos, yPos)) count++;
			}
			xPos = width - ring - 1;
			for (size_t yPos = ring; yPos < height - ring; yPos++) {
				if (mark(xPos, yPos)) count++;
			}

			size_t yPos = ring;
			for (size_t xPos = ring; xPos < width - ring; xPos++) {
				if (mark(xPos, yPos)) count++;
			}
			yPos = height - ring - 1;
			for (size_t xPos = ring; xPos < width - ring; xPos++) {
				if (mark(xPos, yPos)) count++;
			}
		}

		return targets;

	}
};

auto distance(int x1, int y1, int x2,
	int y2) -> std::pair<double, double> {
	return { (x2 - x1), (y2 - y1) };
};

auto atanP(std::pair<double, double> p) -> double {
	return atan2(p.second, p.first);
};

//Elements are sorted by their angle to origin, starting from zero.
auto sort(std::vector<std::pair<size_t, size_t>>& targets, std::pair<size_t, size_t> origin) {
	std::sort(targets.begin(), targets.end(), [&](auto lhs, auto rhs) {
		auto p1 = atanP(distance(origin.first, origin.second, lhs.first, lhs.second));
		auto p2 = atanP(distance(origin.first, origin.second, rhs.first, rhs.second));
		return p1 < p2;
		});

};

void day10() {
	std::string input = R"(#..#....#...#.#..#.......##.#.####
#......#..#.#..####.....#..#...##.
.##.......#..#.#....#.#..#.#....#.
###..#.....###.#....##.....#...#..
...#.##..#.###.......#....#....###
.####...##...........##..#..#.##..
..#...#.#.#.###....#.#...##.....#.
......#.....#..#...##.#..##.#..###
...###.#....#..##.#.#.#....#...###
..#.###.####..###.#.##..#.##.###..
...##...#.#..##.#............##.##
....#.##.##.##..#......##.........
.#..#.#..#.##......##...#.#.#...##
.##.....#.#.##...#.#.#...#..###...
#.#.#..##......#...#...#.......#..
#.......#..#####.###.#..#..#.#.#..
.#......##......##...#..#..#..###.
#.#...#..#....##.#....#.##.#....#.
....#..#....##..#...##..#..#.#.##.
#.#.#.#.##.#.#..###.......#....###
...#.#..##....###.####.#..#.#..#..
#....##..#...##.#.#.........##.#..
.#....#.#...#.#.........#..#......
...#..###...#...#.#.#...#.#..##.##
.####.##.#..#.#.#.#...#.##......#.
.##....##..#.#.#.......#.....####.
#.##.##....#...#..#.#..###..#.###.
...###.#..#.....#.#.#.#....#....#.
......#...#.........##....#....##.
.....#.....#..#.##.#.###.#..##....
.#.....#.#.....#####.....##..#....
.####.##...#.......####..#....##..
.#.#.......#......#.##..##.#.#..##
......##.....##...##.##...##......)";
	std::istringstream reader(input);

	std::vector<std::string> lines;
	for (std::string line; std::getline(reader, line);) {
		lines.emplace_back(line);
	}
	auto s = space(lines);

	size_t highest = 0;
	size_t originX = 0;
	size_t originY = 0;

	for (auto y = 0; y < s.height; y++) {
		for (auto x = 0; x < s.width; x++) {
			auto seen = s.visible(x, y);
			if (seen.size() > highest) {
				highest = seen.size();
				originX = x;
				originY = y;
			}
		}
	}

	size_t destroyed = 0;
	while (true) {
		auto asteroids = s.visible(originX, originY);
		if (asteroids.size() == 0)
			break;
		sort(asteroids, { originX,originY });

		//std::vector<std::string> grid(5, std::string(17, '.'));

		//TODO, this has to find the value closest to zero (but above) if there is no exact zero
		auto find = [&](auto p) {
			auto val = std::abs(atanP(distance(originX, originY, p.first, p.second))) - M_PI_2;
			return val;
		};

		double closestRes = M_PI * 2;
		std::vector<std::pair<size_t, size_t>>::iterator piOver2 = asteroids.begin();
		for (auto i = asteroids.begin(); i < asteroids.end(); i++) {
			auto res = find(*i);
			//This isn't all that valid since we really should reject negative values unless a minor floating point math glitch
			//pushes a number to be negative!
			if (res >= 0 && res < closestRes) {
				closestRes = res;
				piOver2 = i;
			}
		}
		std::rotate(asteroids.begin(), piOver2, asteroids.end());
		if (destroyed + asteroids.size() >= 200) {
			auto fin = asteroids[200 - destroyed - 1];
			std::cout << "DONE\n";
			std::cout << fin.first << ',' << fin.second << "\n";
			return;
		}
		s.destroy(asteroids);
		destroyed += asteroids.size();
	}
	//for (size_t i = 0; i < asteroids.size(); i++) {
	//	//std::cout << "X:" << a.first << ',' << "Y:" << a.second << '\n';
	//	grid[asteroids[i].second][asteroids[i].first] = char(size_t('A') + i);
	//}

	//for (auto x : grid) {
	//	std::cout << x << '\n';
	//}
	std::cout << destroyed << '\n';
}