#ifndef MODE_H
#define MODE_H
#include "raylib.h"

class Mode {
public:
    virtual ~Mode() = default;
    Mode();
    virtual void Draw() = 0;
    virtual void HandleInput() = 0;
    // true if the player has chosen to leave the mode
    bool exitMode;
    // the next mode the player will move to
    int nextMode;
protected:
    Music music;
    Font font;
};

#endif