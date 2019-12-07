#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/split.hpp>
#include <exception>
#include <functional>
#include <iostream>
#include <list>
#include <set>
#include <tuple>
#include <vector>
#include <filesystem>
#include <fstream>
#include <optional>
#include "day6.h"

using namespace boost::algorithm;

struct planet {
	std::string name;
	std::set<planet> orbiters;
	planet(std::string s) : name(s) {}
	planet(std::string s, std::set<planet> p) : name(s), orbiters(p) {}
	bool operator<(planet const& rhs) const { return rhs.name < name; }
};

using universe = std::vector<planet>;

using orbit = std::pair<std::string, std::string>;

auto getOrbiters(std::vector<orbit> const& orbits, std::string const& source)
-> std::set<planet> {
	std::set<planet> out;
	for (auto& x : orbits) {
		if (x.first == source) out.insert(planet(x.second));
	}
	return out;
}

auto populatePlanet(std::vector<orbit> const& allOrbits, planet const& p) -> std::optional<planet> {
	auto betas = getOrbiters(allOrbits, p.name);
	if (betas.size() == 0)
		return planet{ p.name };

	std::set<planet> orbs;
	for (auto i = betas.begin(); i != betas.end(); i++) {
		auto orb = populatePlanet(allOrbits, *i);
		if (orb.has_value())
			orbs.insert(orb.value());
	}

	if (orbs.size() == 0)
		return planet{ p.name };
	else
		return planet{ p.name,orbs };
};

auto sumOfDepth(planet const& p, size_t depth = 0) -> size_t {
	size_t sum = depth;

	for (auto const& x : p.orbiters) sum += sumOfDepth(x, depth + 1);

	return sum;
}

auto hasKey(planet const& p, std::string const& key) -> bool {
	if (p.name == key) return true;
	if (p.orbiters.size() == 0)
		return false;
	for (auto const& x : p.orbiters)
		if (hasKey(x, key))
			return true;
	return false;
}

auto hasBoth(planet const& p) -> bool {
	return hasKey(p, std::string("YOU")) && hasKey(p, std::string("SAN"));
}

auto distance(planet const& p, size_t depth = 1) -> size_t {
	if (!hasBoth(p))
		return depth;
	for (auto const& x : p.orbiters) {
		if (hasBoth(x))
			return distance(x, depth + 1);
	}
	//We hit the last common element, now what?
	return 0;
}

void day6() {
	auto fileName = "sample_day6.txt";
	std::ifstream file(fileName);
	const auto fileSize = std::filesystem::file_size(fileName);

	auto input = std::string(size_t(fileSize),'\0');
	file.read(input.data(), input.size());
	auto read = file.gcount();
	input.resize(read);

	std::vector<std::string> lines;
	split(lines, input, is_any_of("\n"));

	std::vector<orbit> allOrbits;
	for (auto l : lines) {
		std::vector<std::string> sides;
		split(sides, l, is_any_of(")"));
		if (sides.size() != 2)
			throw std::length_error("Not the right number of planets");

		allOrbits.emplace_back(orbit{ sides[0], sides[1] });
	}
	universe localUniverse;

	localUniverse.emplace_back(planet("COM"));

	for (auto& p : localUniverse) {
		p = populatePlanet(allOrbits, p).value();
	}

	std::cout << std::boolalpha;

	std::cout << sumOfDepth(localUniverse.back()) << '\n';

	std::cout << distance(localUniverse.back()) << '\n';
	std::cout << hasBoth(localUniverse.back()) << '\n';
};