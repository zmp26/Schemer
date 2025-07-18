#pragma once
#include "LevelScheme.h"
#include <string>
#include <vector>
#include <cairo/cairo.h>

class CairoRenderer {
public:
	static void RenderToPDF(const LevelScheme& scheme, const std::string& filename);
	static void RenderToSVG(const LevelScheme& scheme, const std::string& filename);

private:
	static void Render(const LevelScheme& scheme, cairo_t* cr);
};