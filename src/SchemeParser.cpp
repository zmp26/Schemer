#include "SchemeParser.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>

static std::string trim(const std::string& s){
	auto start = s.begin();
	while(start != s.end() && std::isspace(*start)) ++ start;
	auto end = s.end();
	do { --end; } while (std::distance(start,end) > 0 && std::isspace(*end));
	return std::string(start, end+1);
}

std::vector<LevelScheme> SchemeParser::ParseFile(const std::string& filename){
	std::ifstream in(filename);
	if(!in) throw std::runtime_error("Failed to open input file: " + filename);

	std::vector<LevelScheme> schemes;
	LevelScheme current;
	std::string line;

	while(std::getline(in,line)){
		if(line.empty() || line[0] == '#') continue;

		if(line == "===") {
			if(!current.getLevels().empty()){
				schemes.push_back(current);
				current = LevelScheme{};
			}
			continue;
		}

		std::istringstream iss(line);
		std::string tag;
		iss >> tag;

		if(tag == "ISOTOPE:"){
			std::string isotope;
			std::getline(iss,isotope);
			current.setIsotopeLabel(trim(isotope));
		} else if(tag == "LEVEL:"){
			double energy;
			std::string spinParity, label;
			iss >> energy >> spinParity >> label;
			current.addLevel(energy, spinParity, label);
		} else if(tag == "THRESHOLD:"){
			double energy, r, g, b;
			std::string type;
			iss >> energy >> type >> r >> g >> b;
			std::vector<double> dash;
			double val;
			while(iss >> val) dash.push_back(val);
			current.addThreshold(energy,type,dash,r,g,b);
		} else if(tag == "TRANSITION:"){
			double from, to, intensity;
			std::string type;
			iss >> from >> to >> type >> intensity;
			current.addTransition(from,to,type,intensity);
		}
	}

	if(!current.getLevels().empty()){
		schemes.push_back(current);
	}

	return schemes;
}