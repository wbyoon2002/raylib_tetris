#include "mode.h"
#include <string>

Mode::Mode(): exitMode(false), nextMode(0) {
    std::string assetsBasePath = ASSETS_PATH;
    std::string fontPath = assetsBasePath + "/Font/monogram.ttf";
    font = LoadFontEx(fontPath.c_str(), 64, 0, 0);
}