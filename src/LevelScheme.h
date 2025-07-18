#pragma once
#include <string>
#include <vector>

struct Threshold{
	double energy;//energy in MeV at which the decay threshold opens up
	std::string type;//type of threshold (i.e. alpha + d)
	std::vector<double> dashPattern = {4.,2.};//default to dashed line
	double r=1., g=0., b=0.;//default to red
};

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
	void addThreshold(double energy, const std::string& type);
	void addThreshold(double energy, const std::string& type, const std::vector<double>& dash, double r, double g, double b);
	void printScheme() const;

	const std::vector<Level>& getLevels() const;
	const std::vector<Transition>& getTransitions() const;
	const std::vector<Threshold>& getThresholds() const;

	std::string getIsotopeLabel() const {
		return isotopeLabel;
	}

	void setIsotopeLabel(const std::string& label) {
		isotopeLabel = label;
	}

private:
	std::vector<Level> levels;
	std::vector<Transition> transitions;
	std::vector<Threshold> thresholds;
	std::string isotopeLabel;
};