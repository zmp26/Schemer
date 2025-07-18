#include "LevelScheme.h"
#include <iostream>
#include <fstream>

void LevelScheme::addLevel(double energy, const std::string& spinParity, const std::string& label){
	levels.push_back({energy,spinParity,label});
}

void LevelScheme::addTransition(double from, double to, const std::string& type, double intensity){
	transitions.push_back({from,to,type,intensity});
}

void LevelScheme::addThreshold(double energy, const std::string& type){
	thresholds.push_back({energy,type});
}

void LevelScheme::addThreshold(double energy, const std::string& type, const std::vector<double>& dash, double r, double g, double b){
	thresholds.push_back({energy,type,dash,r,g,b});
}

void LevelScheme::printScheme() const {
	std::cout << "Nuclear Level Scheme:\n";
	for(const auto& lvl : levels){
		std::cout << "\tLevel: " << lvl.energy << " keV, " << lvl.spinParity << " (" << lvl.label << ")\n";
	}
	for(const auto& transition : transitions){
		std::cout << "\tTransition: " << transition.fromEnergy << " -> " << transition.toEnergy << " [" << transition.type << "], Intensity: " << transition.intensity << "\n";
	}

}

const std::vector<Level>& LevelScheme::getLevels() const { return levels; }
const std::vector<Transition>& LevelScheme::getTransitions() const { return transitions; }
const std::vector<Threshold>& LevelScheme::getThresholds() const { return thresholds; }