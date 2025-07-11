#include "LevelScheme.h"
#include "CairoRenderer.h"
#include <iostream>

int main() {
    LevelScheme scheme;
    scheme.setIsotopeLabel("^123Ab");

    scheme.addLevel(0.0, "0^+", "GS");
    scheme.addLevel(1225.1, "2^+", "1st Exc");
    scheme.addLevel(5006.5, "4^+", "2nd Exc");

    scheme.addTransition(1225.1, 0.0, "E2", 100.0);
    scheme.addTransition(5006.5, 1225.1, "E2", 80.0);

    CairoRenderer::renderToPDF(scheme, "nuclear_scheme_test.pdf");

    std::cout << "Exported to nuclear_scheme_test.pdf (vector graphics).\n";
    return 0;
}