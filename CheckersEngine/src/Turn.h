#pragma once

#include <vector>

#include "Move.h"

class Turn {
public:
    std::vector<Move> moves;

    Turn(Move move);

    bool operator==(const Turn& other) const;

    Turn addMove(Move move) const;
    bool isCapturing() const;
    bool startsWith(Turn other) const;

private:
    Turn(std::vector<Move> moves);
};