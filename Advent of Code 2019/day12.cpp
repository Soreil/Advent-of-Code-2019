#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include <set>
#include <iostream>
#include <boost/algorithm/string.hpp>
#include <boost/container_hash/hash.hpp>
#include "day12.h"
using namespace boost::algorithm;

template<typename T>
struct position {
	T x = 0;
	T y = 0;
	T z = 0;

	auto operator+(position const rhs) const {
		position pos;
		pos.x = x + rhs.x;
		pos.y = y + rhs.y;
		pos.z = z + rhs.z;
		return pos;
	}

	auto operator==(position const rhs) const {
		return x == rhs.x && y == rhs.y && z == rhs.z;
	}

	auto sumAbs() {
		T sum = 0;
		sum += std::abs(x);
		sum += std::abs(y);
		sum += std::abs(z);
		return sum;
	}


	position(T const x_, T const y_, T const z_) : x(x_), y(y_), z(z_) {}
	position() {}
};

template <typename T>
auto hash_value(position<T> h) {
	std::vector<T> coordinates = { h.x,h.y,h.z };
	return boost::hash_range(coordinates.begin(), coordinates.end());
}

template <typename T>
auto potentialEnergy(position<T> moon) {
	T potential = 0;
	potential += moon.sumAbs();
	return potential;
}
template <typename T>
auto kineticEnergy(position<T> velocity) {
	T potential = 0;
	potential += velocity.sumAbs();
	return potential;
}
template <typename T>
auto energy(std::vector < position<T>> moons, std::vector < position<T>> velocities) {
	T totalEnergy = 0;
	for (size_t i = 0; i < moons.size(); i++)
		totalEnergy += potentialEnergy(moons[i]) * kineticEnergy(velocities[i]);
	return totalEnergy;
}

template<typename T>
auto updateGravity(std::vector<position<T>> moons, std::vector<position<T>> velocities) {
	for (size_t i = 0; i < moons.size(); i++) {
		for (auto j = i + 1; j < moons.size(); j++) {
			auto difference = moons[i].x - moons[j].x;
			if (difference < 0) {
				velocities[i].x++;
				velocities[j].x--;
			}
			else if (difference > 0) {
				velocities[i].x--;
				velocities[j].x++;
			}

			difference = moons[i].y - moons[j].y;
			if (difference < 0) {
				velocities[i].y++;
				velocities[j].y--;
			}
			else if (difference > 0) {
				velocities[i].y--;
				velocities[j].y++;
			}

			difference = moons[i].z - moons[j].z;
			if (difference < 0) {
				velocities[i].z++;
				velocities[j].z--;
			}
			else if (difference > 0) {
				velocities[i].z--;
				velocities[j].z++;
			}
		}
	}

	return velocities;
}

void day12() {
				std::string in = R"(<x=-3, y=15, z=-11>
<x=3, y=13, z=-19>
<x=-13, y=18, z=-2>
<x=6, y=0, z=-1>)";
//	std::string in = R"(<x=-1, y=0, z=2>
//<x=2, y=-10, z=-7>
//<x=4, y=-8, z=8>
//<x=3, y=5, z=-1>)";
	//		std::string in = R"(<x=-8, y=-10, z=0>
	//<x=5, y=5, z=10>
	//<x=2, y=-7, z=3>
	//<x=9, y=-8, z=-3>)";

	std::vector<position<int>> moons;

	std::vector<std::string> lines;
	split(lines, in, is_any_of("\n"));
	for (auto line : lines) {
		erase_head(line, 1);
		erase_tail(line, 1);
		erase_all(line, " ");
		std::vector<std::string> tokens;
		split(tokens, line, is_any_of(","));

		std::vector<int> numbers;
		for (auto t : tokens) {
			erase_head(t, 2);
			numbers.emplace_back(std::stoi(t));
		}
		moons.emplace_back(numbers[0], numbers[1], numbers[2]);
	}

	std::vector<position<int>> velocity(moons.size());

	auto origin = moons;
	std::vector<int> Xorigin;
	std::vector<int> Yorigin;
	std::vector<int> Zorigin;
	for (auto o : origin) {
		Xorigin.emplace_back(o.x);
		Yorigin.emplace_back(o.y);
		Zorigin.emplace_back(o.z);
	}
	size_t repeated = 0;
	for (size_t i = 0; i < 3000000; i++) {
		velocity = updateGravity(moons, velocity);
		std::transform(moons.begin(), moons.end(), velocity.begin(), moons.begin(), std::plus<>());
		{
			bool xFound = true;
			for (size_t j = 0; j < moons.size(); j++) {
				if (Xorigin[j] != moons[j].x)
					xFound = false;
				if (velocity[j].x != 0)
					xFound = false;
			}
			if (xFound)
				std::cout << "X:" << i+1 << '\n';
		}
		{
			bool yFound = true;
			for (size_t j = 0; j < moons.size(); j++) {
				if (Yorigin[j] != moons[j].y)
					yFound = false;
				if (velocity[j].y != 0)
					yFound = false;
			}
			if (yFound)
				std::cout << "Y:" << i+1 << '\n';
		}
		{
			bool zFound = true;
			for (size_t j = 0; j < moons.size(); j++) {
				if (Zorigin[j] != moons[j].z)
					zFound = false;
				if (velocity[j].z != 0)
					zFound = false;
			}
			if (zFound)
				std::cout << "Z:" << i+1 << '\n';
		}
		//Yorigin.emplace_back(o.y);
		//Zorigin.emplace_back(o.z);
	}



	std::cout << repeated << '\n';
}