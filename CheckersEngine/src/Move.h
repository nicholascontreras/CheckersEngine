#pragma once

#include "Coord.h"

class Move {
public:
    const Coord from, to;

    Move(Coord from, Coord to);

    bool operator==(const Move& other) const;

    bool isCapture() const;
    Coord getCapturedCoord() const;
};