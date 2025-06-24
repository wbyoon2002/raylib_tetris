#include "position.h"

Position::Position(int row, int column) : row(row), column(column) {}

bool Position::operator==(const Position& other) const {
    return (row == other.row) && (column == other.column);
}