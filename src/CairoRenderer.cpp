#include "CairoRenderer.h"
#include <cairo/cairo.h>
#include <cairo/cairo-pdf.h>
#include <cairo/cairo-svg.h>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <cctype>

constexpr double WIDTH = 600;
constexpr double HEIGHT = 800;
constexpr double MARGIN = 50;
constexpr double LEVEL_WIDTH = 200;
constexpr double TEXT_OFFSET = 5;

void drawText(cairo_t* cr, double x, double y, const std::string& text, double size = 12){
	cairo_move_to(cr,x,y);
	cairo_select_font_face(cr, "Liberation Serif", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
	cairo_set_font_size(cr,size);
	cairo_show_text(cr, text.c_str());
}

void drawTextItalic(cairo_t* cr, double x, double y, const std::string& text, double size = 12){
	cairo_move_to(cr,x,y);
	cairo_select_font_face(cr, "Liberation Serif", CAIRO_FONT_SLANT_ITALIC, CAIRO_FONT_WEIGHT_NORMAL);
	cairo_set_font_size(cr,size);
	cairo_show_text(cr,text.c_str());
}

double drawSuperscriptedLabel(cairo_t* cr, const std::string& text, double x, double y, double size=16){
	cairo_save(cr);
	cairo_select_font_face(cr,"Serif",CAIRO_FONT_SLANT_NORMAL,CAIRO_FONT_WEIGHT_NORMAL);
	cairo_set_font_size(cr,size);

	double currX = x;
	bool inSuper = false;

	for(size_t i=0; i<text.length(); ++i){
		char c = text[i];

		if(c == '^'){
			inSuper = true;
			continue;
		}

		std::string ch(1,c);
		cairo_text_extents_t ext;

		if(inSuper && (std::isdigit(c) || c == '+' || c == '-')){
			double scale = 0.7;
			double dy = -size*0.4;
			cairo_save(cr);
			cairo_set_font_size(cr,size*scale);
			cairo_text_extents(cr,ch.c_str(),&ext);
			cairo_move_to(cr,currX,y+dy);
			cairo_show_text(cr,ch.c_str());
			cairo_restore(cr);
			currX += ext.x_advance;
			//inSuper = false;
		} else {
			inSuper = false;

			cairo_text_extents(cr,ch.c_str(),&ext);
			cairo_move_to(cr,currX,y);
			cairo_show_text(cr,ch.c_str());
			currX += ext.x_advance;
		}
	}

	cairo_restore(cr);
	return currX - x;
}

void drawArrowhead(cairo_t* cr, double x, double y, bool pointingUp, double size = 10){
	cairo_save(cr);

	cairo_move_to(cr,x,y);
	if(pointingUp){
		cairo_line_to(cr,x-size/2,y+size);
		cairo_line_to(cr,x+size/2,y+size);
	} else {
		cairo_line_to(cr,x-size/2,y-size);
		cairo_line_to(cr,x+size/2,y-size);
	}
	cairo_close_path(cr);

	cairo_set_source_rgb(cr,0,0,0);
	cairo_fill(cr);

	cairo_restore(cr);
}

void CairoRenderer::Render(const LevelScheme& scheme, cairo_t* cr){
	cairo_select_font_face(cr, "Liberation Serif", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
	cairo_set_line_width(cr, 2.0);

	const auto& levels = scheme.getLevels();
	const auto& transitions = scheme.getTransitions();
	const auto& thresholds = scheme.getThresholds();

	double maxEnergy = 0.;
	for(const auto& l : levels) maxEnergy = std::max(maxEnergy,l.energy);
	for(const auto& t : thresholds) maxEnergy = std::max(maxEnergy,t.energy);

	double scaleY = (HEIGHT - 2 * MARGIN) / (maxEnergy + 100.);

	for(const auto& t : thresholds){
		double y = HEIGHT - MARGIN - t.energy * scaleY;
		cairo_set_source_rgb(cr, t.r, t.g, t.b);
		
		if(!t.dashPattern.empty()){
			cairo_set_dash(cr, t.dashPattern.data(), t.dashPattern.size(), 0);
		} else {
			cairo_set_dash(cr, nullptr, 0, 0);
		}

		double dashLineStartX = MARGIN + 0.1*(WIDTH-2*MARGIN);
		double dashLineEndX = WIDTH - MARGIN - 0.1*(WIDTH-2*MARGIN);

		cairo_move_to(cr, dashLineStartX, y);
		cairo_line_to(cr,dashLineEndX,y);
		cairo_stroke(cr);

		cairo_set_dash(cr, nullptr, 0, 0);

		drawTextItalic(cr, dashLineEndX + 5, y + 4, t.type, 10);

		cairo_text_extents_t ext;
		cairo_text_extents(cr,t.type.c_str(), &ext);
		double energyX = dashLineEndX + 5 + ext.x_advance + 5;

		std::ostringstream energyStr;
		energyStr << t.energy << " keV";
		drawText(cr, energyX, y+4, energyStr.str(), 10);

		// cairo_save(cr);
		// cairo_select_font_face(cr,"Serif",CAIRO_FONT_SLANT_ITALIC,CAIRO_FONT_WEIGHT_NORMAL);
		// drawText(cr, WIDTH-MARGIN+5,y+4,t.type,10);
		// cairo_restore(cr);
	}

	for(const auto& lvl : levels){
		double y = HEIGHT - MARGIN - lvl.energy * scaleY;
		double x1 = (WIDTH - LEVEL_WIDTH) / 2;
		double x2 = x1 + LEVEL_WIDTH;

		cairo_set_source_rgb(cr, 0, 0, 0);
		cairo_move_to(cr, x1, y);
		cairo_line_to(cr, x2, y);
		cairo_stroke(cr);

		double currX = x2 + TEXT_OFFSET;

		std::string prefix = lvl.label + " (";
		drawText(cr, currX, y+4, prefix);

		cairo_text_extents_t ext;
		cairo_text_extents(cr, prefix.c_str(), &ext);
		currX += ext.x_advance;

		currX += drawSuperscriptedLabel(cr, lvl.spinParity, currX, y+4, 12);

		std::string suffix = "), ";
		drawText(cr, currX, y+4, suffix);
		cairo_text_extents(cr, suffix.c_str(), &ext);
		currX += ext.x_advance;

		std::ostringstream energyStr;
		energyStr << lvl.energy << " keV";
		drawText(cr, currX, y+4, energyStr.str());
	}

	for(const auto& t : transitions){
		double y1 = HEIGHT - MARGIN - t.fromEnergy*scaleY;
		double y2 = HEIGHT - MARGIN - t.toEnergy*scaleY;
		double x = WIDTH/2;

		bool pointingUp = y2 < y1;
		double arrowSize = 8;
		double y2LineEnd = pointingUp ? (y2 + arrowSize) : (y2 - arrowSize);

		cairo_move_to(cr, x, y1);
		cairo_line_to(cr, x, y2LineEnd);
		cairo_stroke(cr);

		drawArrowhead(cr, x, y2, pointingUp, arrowSize);

		std::ostringstream label;
		label << t.type << ", " << t.intensity << "%";
		drawText(cr, x+TEXT_OFFSET,(y1+y2)/2,label.str(),10);
	}

	std::string isotopeLabel = scheme.getIsotopeLabel();
	cairo_text_extents_t ext;
	cairo_set_font_size(cr,16);
	cairo_text_extents(cr,isotopeLabel.c_str(),&ext);
	double labelX = (WIDTH - ext.width)/2;
	drawSuperscriptedLabel(cr, isotopeLabel, labelX, HEIGHT-15);
}

void CairoRenderer::RenderToPDF(const LevelScheme& scheme, const std::string& filename){
	cairo_surface_t* surface = cairo_pdf_surface_create(filename.c_str(), WIDTH, HEIGHT);
	cairo_t* cr = cairo_create(surface);
	Render(scheme,cr);
	cairo_destroy(cr);
	cairo_surface_destroy(surface);
}

void CairoRenderer::RenderToSVG(const LevelScheme& scheme, const std::string& filename){
	cairo_surface_t* surface = cairo_svg_surface_create(filename.c_str(), WIDTH, HEIGHT);
	cairo_t* cr = cairo_create(surface);
	Render(scheme,cr);
	cairo_destroy(cr);
	cairo_surface_destroy(surface);
}