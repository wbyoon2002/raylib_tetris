#include "grid.h"
#include "colors.h"
#include "interface.h"

Grid::Grid(float x, float y) : numRows(20), numCols(10), cellSize(30), offsetX(x), offsetY(y), rng(rd()) {
    Initialize();
    colors = GetCellColors();
}

void Grid::Initialize() {
    for (int row = 0; row < numRows; row++) {
        for (int column = 0; column < numCols; column++) {
            grid[row][column] = 0;
        }
    }

    // To demonstrate SRS
    /*
    grid[19][9] = 8;
    grid[19][8] = 8;
    grid[19][7] = 8;
    grid[19][6] = 8;
    grid[19][4] = 8;
    grid[19][3] = 8;
    grid[19][2] = 8;
    grid[19][1] = 8;
    grid[19][0] = 8;
    grid[18][0] = 8;
    grid[18][1] = 8;
    grid[18][2] = 8;
    grid[18][3] = 8;
    grid[18][6] = 8;
    grid[18][7] = 8;
    grid[18][8] = 8;
    grid[18][9] = 8;
    grid[17][9] = 8;
    grid[17][8] = 8;
    grid[17][7] = 8;
    grid[17][6] = 8;
    grid[17][1] = 8;
    grid[17][0] = 8;
    grid[16][1] = 8;
    grid[16][2] = 8;
    grid[16][3] = 8;
    grid[16][7] = 8;
    grid[16][8] = 8;
    grid[16][9] = 8;
    grid[15][9] = 8;
    grid[15][8] = 8;
    grid[15][7] = 8;
    grid[15][6] = 8;
    grid[14][5] = 8;
    grid[14][6] = 8;
    grid[14][7] = 8;
    grid[13][4] = 8;
    grid[13][5] = 8;
    */
    // To demonstrate back-to-back
    /*
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 9; j++) {
            grid[numRows - 1 - i][j] = 8;
        }
    }
    */
    // To demonstrate Mini T-Spin no line(s)
    /*
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 10; j++) {
            grid[numRows - i - 1][j] = 8;
        }
    }
    grid[numRows - 1][numCols - 1] = 0;
    grid[numRows - 2][5] = 0;
    grid[numRows - 2][6] = 0;
    grid[numRows - 2][7] = 0;
    grid[numRows - 2][9] = 0;
    grid[numRows - 3][4] = 8;
    grid[numRows - 3][5] = 8;
    grid[numRows - 3][8] = 8;
    grid[numRows - 3][9] = 8;
    grid[numRows - 4][9] = 8;
    */
    // To demonstrate T-spin no line(s)
    /*
    for (int i = 0; i < 10; i++) {
        grid[numRows - 1][i] = 8;
    }
    grid[numRows - 1][0] = 0;
    grid[numRows - 1][5] = 0;
    grid[numRows - 2][3] = 8;
    grid[numRows - 2][7] = 8;
    grid[numRows - 2][8] = 8;
    grid[numRows - 3][6] = 8;
    grid[numRows - 3][7] = 8;
    */
    // To demonstrate Mini T-spin single
    /*
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 10; j++) {
            grid[numRows - i - 1][j] = 8;
        }
    }
    grid[numRows - 1][numCols - 1] = 0;
    grid[numRows - 2][5] = 0;
    grid[numRows - 2][6] = 0;
    grid[numRows - 2][7] = 0;
    grid[numRows - 3][4] = 8;
    grid[numRows - 3][5] = 8;
    grid[numRows - 3][8] = 8;
    grid[numRows - 3][9] = 8;
    grid[numRows - 4][9] = 8;
    */
    // To demonstrate T-spin single
    /*
    for (int i = 0; i < 10; i++) {
        grid[numRows - 1][i] = 8;
    }
    grid[numRows - 1][5] = 0;
    grid[numRows - 2][3] = 8;
    grid[numRows - 2][7] = 8;
    grid[numRows - 2][8] = 8;
    grid[numRows - 3][6] = 8;
    grid[numRows - 3][7] = 8;
    */
    // To demonstrate Mini T-spin double
    /*
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 10; j++) {
            grid[numRows - i - 1][j] = 8;
        }
    }
    grid[numRows - 2][5] = 0;
    grid[numRows - 2][6] = 0;
    grid[numRows - 2][7] = 0;
    grid[numRows - 3][4] = 8;
    grid[numRows - 3][5] = 8;
    grid[numRows - 3][8] = 8;
    grid[numRows - 3][9] = 8;
    grid[numRows - 4][9] = 8;
    */
    // To demonstrate T-spin double (with difficult wall kick)
    /*
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 10; j++) {
            grid[numRows - i - 1][j] = 8;
        }
    }
    grid[numRows - 1][numCols - 3] = 0;
    grid[numRows - 1][numCols - 2] = 0;
    grid[numRows - 2][numCols - 3] = 0;
    grid[numRows - 2][numCols - 2] = 0;
    grid[numRows - 3][numCols - 2] = 0;
    grid[numRows - 4][numCols - 1] = 8;
    grid[numRows - 5][numCols - 1] = 8;
    grid[numRows - 5][numCols - 2] = 8;
    */
    // To demonstrate T-spin triple
    /*
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 10; j++) {
            grid[numRows - i - 1][j] = 8;
        }
    }
    grid[numRows - 1][1] = 0;
    grid[numRows - 2][1] = 0;
    grid[numRows - 2][2] = 0;
    grid[numRows - 3][1] = 0;
    grid[numRows - 4][1] = 0;
    grid[numRows - 4][2] = 0;
    grid[numRows - 4][3] = 0;
    grid[numRows - 5][2] = 0;
    grid[numRows - 5][3] = 0;
    */
}

void Grid::Draw() {
    for (int row = 0; row < numRows; row++) {
        for (int column = 0; column < numCols; column++) {
            int cellValue = grid[row][column];
            DrawRectangle(column * cellSize + offsetX + 191, row * cellSize + offsetY + 11, cellSize - 1, cellSize - 1, colors[cellValue]);
        }
    }
}

bool Grid::IsCellOutside(int row, int column)
{
    if (row >= 0 && row < numRows && column >= 0 && column < numCols) {
        return false;
    }
    return true;
}

bool Grid::IsCellEmpty(int row, int column)
{
    if (grid[row][column] == 0) {
        return true;
    }
    return false;
}

int Grid::ClearFullRows()
{
    int completed = 0;
    for (int row = numRows - 1; row >= 0; row--) {
        if (IsRowFull(row)) {
            ClearRow(row);
            completed++;
        }
        else if (completed > 0) {
            MoveRowDown(row, completed);
        }
    }
    return completed;
}

int Grid::CountFullRows()
{
    int completed = 0;
    for (int row = numRows - 1; row >= 0; row--) {
        if (IsRowFull(row)) {
            completed++;
        }
    }
    return completed;
}

bool Grid::CreateGarbageRows(int garbageRows)
{
    bool result = true;
    std::uniform_int_distribution<int> dist(0, numCols - 1);
    int randomIndex = dist(rng);
    for (int row = 0; row < numRows - garbageRows; row++) {
        if (row < garbageRows && !IsRowEmpty(row)) {
            result = false;
        }
        MoveRowUp(row, garbageRows);
    }
    for (int row = 0; row < garbageRows; row++) {
        for (int column = 0; column < numCols; column++) {
            if (column != randomIndex) {
                grid[numRows - row - 1][column] = 8;
            }
        }
    }
    return result;
}

bool Grid::IsRowFull(int row)
{
    for (int column = 0; column < numCols; column++) {
        if (grid[row][column] == 0) {
            return false;
        }
    }
    return true;
}

void Grid::ClearRow(int row) {
    for (int column = 0; column < numCols; column++) {
        grid[row][column] = 0;
    }
}

void Grid::MoveRowDown(int row, int numRows) {
    for (int column = 0; column < numCols; column++) {
        grid[row + numRows][column] = grid[row][column];
        grid[row][column] = 0;
    }
}

bool Grid::IsRowEmpty(int rowIndex) {
    for (int i = 0; i < numCols; i++) {
        if (grid[rowIndex][i] != 0) {
            return false;
        }
    }
    return true;
}

void Grid::MoveRowUp(int row, int numRows) {
    for (int column = 0; column < numCols; column++) {
        grid[row][column] = grid[row + numRows][column];
        grid[row + numRows][column] = 0;
    }
}