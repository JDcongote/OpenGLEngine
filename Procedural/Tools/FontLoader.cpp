#pragma once
#include "FontLoader.h"

void load_font(std::string name) {
	name = "Resources/Fonts/" + name + ".fnt";
	std::string line;
	std::ifstream file(name);
	if (file.is_open()) {
		while (getline(file, line)) {
			
		}
	}
}