#include "Move.h"

#include <stdexcept>

Move::Move(Coord from, Coord to) : from(from), to(to) {
    if(!from.isOnBoard() || !to.isOnBoard()) {
        throw std::invalid_argument("Coords must be on the Board!");
    }
}

bool Move::operator==(const Move& other) const {
    return from == other.from && to == other.to;
}

bool Move::isCapture() const {
    int rowDiff = from.row - to.row;
    return rowDiff == 2 || rowDiff == -2;
}

Coord Move::getCapturedCoord() const {
    if(!isCapture()) {
        throw std::logic_error("Cannot get captured Coord, this is not a capture Move!");
    }

    return {(from.row + to.row) / 2, (from.col + to.col) / 2 };
}
