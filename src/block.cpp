#include "block.h"

Block::Block()
    : cellSize(30), rotationState(0), rowOffset(0), columnOffset(0) {
    colors = GetCellColors();
    /*
    clockwiseWallKick[0] = {Position(0, 0), Position(-1, 0), Position(-1, -1), Position(0, 2), Position(-1, 2)};
    clockwiseWallKick[1] = {Position(0, 0), Position(-1, 0), Position(-1, 1), Position(0, -2), Position(-1, -2)};
    clockwiseWallKick[2] = {Position(0, 0), Position(1, 0), Position(1, -1), Position(0, 2), Position(1, 2)};
    clockwiseWallKick[3] = {Position(0, 0), Position(1, 0), Position(1, 1), Position(0, -2), Position(1, -2)};
    counterclockwiseWallKick[0] = {Position(0, 0), Position(1, 0), Position(1, -1), Position(0, 2), Position(1, 2)};
    counterclockwiseWallKick[1] = {Position(0, 0), Position(-1, 0), Position(-1, 1), Position(0, -2), Position(-1, -2)};
    counterclockwiseWallKick[2] = {Position(0, 0), Position(-1, 0), Position(-1, -1), Position(0, 2), Position(-1, 2)};
    counterclockwiseWallKick[3] = {Position(0, 0), Position(1, 0), Position(1, 1), Position(0, -2), Position(1, -2)};
    */
    clockwiseWallKick[0] = {Position(0, 0), Position(0, -1), Position(1, -1), Position(-2, 0), Position(-2, -1)};
    clockwiseWallKick[1] = {Position(0, 0), Position(0, -1), Position(-1, -1), Position(2, 0), Position(2, -1)};
    clockwiseWallKick[2] = {Position(0, 0), Position(0, 1), Position(1, 1), Position(-2, 0), Position(-2, 1)};
    clockwiseWallKick[3] = {Position(0, 0), Position(0, 1), Position(-1, 1), Position(2, 0), Position(2, 1)};
    counterclockwiseWallKick[0] = {Position(0, 0), Position(0, 1), Position(1, 1), Position(-2, 0), Position(-2, 1)};
    counterclockwiseWallKick[1] = {Position(0, 0), Position(0, -1), Position(-1, -1), Position(2, 0), Position(2, -1)};
    counterclockwiseWallKick[2] = {Position(0, 0), Position(0, -1), Position(1, -1), Position(-2, 0), Position(-2, -1)};
    counterclockwiseWallKick[3] = {Position(0, 0), Position(0, 1), Position(-1, 1), Position(2, 0), Position(2, 1)};
}

void Block::Draw(int offsetX, int offsetY) {
    std::vector<Position> tiles = GetCellPositions();
    for (Position item : tiles) {
        DrawRectangle(item.column * cellSize + offsetX, item.row * cellSize + offsetY, cellSize - 1, cellSize - 1, colors[id]);
    }
}

void Block::DrawEdge(int offsetX, int offsetY) {
    std::vector<Position> tiles = GetCellPositions();
    for (Position item : tiles) {
        DrawRectangleLines(item.column * cellSize + offsetX - 1, item.row * cellSize + offsetY - 1, cellSize + 1, cellSize + 1, WHITE);
    }
}

void Block::Move(int rows, int columns) {
    rowOffset += rows;
    columnOffset += columns;
}

std::vector<Position> Block::GetCellPositions() {
    std::vector<Position> tiles = cells[rotationState];
    std::vector<Position> movedTiles;
    for (Position item : tiles) {
        Position newPos = Position(item.row + rowOffset, item.column + columnOffset);
        movedTiles.push_back(newPos);
    }
    return movedTiles;
}

void Block::RotateClockwise() {
    rotationState = (rotationState + 1) % (int)cells.size();
}

void Block::RotateCounterclockwise() {
    rotationState = (rotationState + (int)cells.size() - 1) % (int)cells.size();
}

void Block::Reset() {
    rotationState = 0;
    switch(id) {
        case 4:
            rowOffset = 0;
            columnOffset = 4;
            break;
        default:
            rowOffset = 0;
            columnOffset = 3;
            break;
    }
}