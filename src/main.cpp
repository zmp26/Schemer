#include "LevelScheme.h"
#include "CairoRenderer.h"
#include "SchemeParser.h"
#include <iostream>
#include <sstream>

// int main() {
//     LevelScheme scheme;
//     scheme.setIsotopeLabel("^123Ab");

//     scheme.addLevel(0.0, "0^+", "GS");
//     scheme.addLevel(1225.1, "2^+", "1st Exc");
//     scheme.addLevel(5006.5, "4^+", "2nd Exc");

//     scheme.addTransition(1225.1, 0.0, "E2", 100.0);
//     scheme.addTransition(5006.5, 1225.1, "E2", 80.0);

//     scheme.addThreshold(2000.7, "α");//default dashed red
//     scheme.addThreshold(3503.4, "p", {8.,4.},0.,0.6,0.);//long-dash green
//     scheme.addThreshold(4044.1, "n", {}, 0., 0., 1.);//solid blue
//     //scheme.addThreshold(6000.0, "p + ^122Ab");

//     CairoRenderer::RenderToPDF(scheme, "nuclear_scheme_test.pdf");
//     CairoRenderer::RenderToSVG(scheme, "nuclear_scheme_test.svg");


//     std::cout << "Exported to nuclear_scheme_test.pdf (vector graphics).\n";
//     return 0;
// }

int main(){
    auto schemes = SchemeParser::ParseFile("../input.txt");
    CairoRenderer renderer;

    for(size_t i=0; i<schemes.size(); i++){
        std::ostringstream fnamepdf, fnamesvg;
        fnamepdf << "scheme_" << i+1 << ".pdf";
        renderer.RenderToPDF(schemes[i], fnamepdf.str());
        fnamesvg << "scheme_" << i+1 << ".svg";
        renderer.RenderToSVG(schemes[i], fnamesvg.str());

        std::cout << "Saved " << fnamepdf.str() << " and " << fnamesvg.str() << "\n";
    }

    return 0;
}