#pragma once

#include <utility>

#include "Direction.h"

class Piece {
public:
    enum class Color { BLACK, RED };

    Color color;
    bool isKing;

    Piece(Color color, bool isKing);

    bool operator==(const Piece other) const;

    std::pair<Direction, Direction> getForwardDirections() const;
    std::pair<Direction, Direction> getBackwardDirections() const;

    static Color getOppositeColor(Color color);

    char toChar() const;
private:
};