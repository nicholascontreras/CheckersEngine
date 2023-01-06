#include "Coord.h"

#include <stdexcept>

#include "Board.h"

Coord::Coord(int row, int col) : row(row), col(col) {
}

bool Coord::operator==(const Coord& other) const {
    return row == other.row && col == other.col;
}

bool Coord::isOnBoard() {
    return row >= 0 && row < Board::SIZE && col >= 0 && col < Board::SIZE;
}

Coord Coord::translate(Direction dir) {
    switch(dir) {
    case Direction::NORTH_EAST:
        return { row - 1, col + 1 };
    case Direction::SOUTH_EAST:
        return { row + 1, col + 1 };
    case Direction::SOUTH_WEST:
        return { row + 1, col - 1 };
    case Direction::NORTH_WEST:
        return { row - 1, col - 1 };
    default:
        throw std::invalid_argument("Invalid Direction!");
    }
}
