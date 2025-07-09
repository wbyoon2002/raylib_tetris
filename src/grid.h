#ifndef GRID_H
#define GRID_H
#include <vector>
#include <random>
#include "raylib.h"

class Grid {
public:
    Grid();
    // sets the values of each element in the grid array
    void Initialize();
    void Draw(float, float);
    bool IsCellOutside(int row, int column);
    bool IsCellEmpty(int row, int column);
    int ClearFullRows();
    int CountFullRows();
    int grid[20][10];
    bool CreateGarbageRows(int);
    private:
    bool IsRowFull(int row);
    void ClearRow(int row);
    void MoveRowDown(int row, int numRows);
    int numRows;
    int numCols;
    int cellSize;
    std::vector<Color> colors;
    // Random number generation members
    std::random_device rd;
    std::mt19937 rng;
    bool IsRowEmpty(int);
    void MoveRowUp(int row, int numRows);
};

#endif