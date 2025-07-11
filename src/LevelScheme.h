#pragma once
#include <string>
#include <vector>

struct Level{
	double energy;
	std::string spinParity;
	std::string label;
};

struct Transition{
	double fromEnergy;
	double toEnergy;
	std::string type;
	double intensity;
};

class LevelScheme{
public:
	void addLevel(double energy, const std::string& spinParity, const std::string& label);
	void addTransition(double from, double to, const std::string& type, double intensity);
	void printScheme() const;

	const std::vector<Level>& getLevels() const;
	const std::vector<Transition>& getTransitions() const;

	std::string getIsotopeLabel() const {
		return isotopeLabel;
	}

	void setIsotopeLabel(const std::string& label) {
		isotopeLabel = label;
	}

private:
	std::vector<Level> levels;
	std::vector<Transition> transitions;
	std::string isotopeLabel;
};