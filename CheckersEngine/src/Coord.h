#pragma once

#include "Direction.h"

class Coord {
public:
    const int row, col;

    Coord(int row, int col);

    bool operator==(const Coord& other) const;

    bool isOnBoard();
    Coord translate(Direction dir);
};
