#ifndef SCHEME_PARSER_H
#define SCHEME_PARSER_H

#include <string>
#include <vector>
#include "LevelScheme.h"

class SchemeParser{
public:
	static std::vector<LevelScheme> ParseFile(const std::string& filename);
};

#endif