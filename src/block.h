#ifndef BLOCK_H
#define BLOCK_H
#include <vector>
#include <map>
#include "position.h"
#include "colors.h"

class Block {
public:
    Block();
    void Draw(int offsetX, int offsetY);
    void DrawEdge(int offsetX, int offsetY);
    void Move(int rows, int columns);
    // returns the positions of each cell according to its offset positions
    std::vector<Position> GetCellPositions();
    void RotateClockwise();
    void RotateCounterclockwise();
    // resets the rotationState and offset values
    void Reset();
    int id;
    int rotationState;
    // a map containing a default position of each cell corresponding to each rotationState
    std::map<int, std::vector<Position>> cells;
    // a map containing shift directions corresponding to each rotationState
    std::map<int, std::vector<Position>> clockwiseWallKick;
    std::map<int, std::vector<Position>> counterclockwiseWallKick;
private:
    // size of each cell in pixels
    int cellSize;
    std::vector<Color> colors;
    int rowOffset;
    int columnOffset;
};

#endif