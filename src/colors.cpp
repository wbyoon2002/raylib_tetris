#include "colors.h"

const Color darkGrey = {36, 40, 50, 255};
const Color green = {47, 230, 23, 255};
const Color red = {232, 18, 18, 255};
const Color orange = {226, 116, 17, 255};
const Color yellow = {237, 234, 4, 255};
const Color purple = {166, 0, 247, 255};
const Color cyan = {21, 204, 209, 255};
const Color blue = {13, 64, 216, 255};
const Color lightBlue = {92, 110, 145, 255};
const Color darkBlue = {16, 18, 28, 255};
const Color garbageGrey = {120, 115, 110, 255};

std::vector<Color> GetCellColors() {
    return {darkGrey, green, red, orange, yellow, purple, cyan, blue, garbageGrey};
}