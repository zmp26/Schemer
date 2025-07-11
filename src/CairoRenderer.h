#pragma once
#include "LevelScheme.h"
#include <string>

class CairoRenderer {
public:
	static void renderToPDF(const LevelScheme& scheme, const std::string& filename);
	static void renderToSVG(const LevelScheme& scheme, const std::string& filename);
};