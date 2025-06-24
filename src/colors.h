#ifndef COLORS_H
#define COLORS_H
#include "raylib.h"
#include <vector>

extern const Color darkGrey;
extern const Color green;
extern const Color red;
extern const Color orange;
extern const Color yellow;
extern const Color purple;
extern const Color cyan;
extern const Color blue;
extern const Color lightBlue;
extern const Color darkBlue;
extern const Color garbageGrey;

// returns the 8 possible colors that can be used for each cell
std::vector<Color> GetCellColors();

#endif