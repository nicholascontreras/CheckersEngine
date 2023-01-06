#pragma once

#include <vector>
#include <optional>
#include <string>

#include "Piece.h"
#include "Coord.h"
#include "Turn.h"
#include "Move.h"

class Board {
public:
    static const int SIZE = 8;

    Board();

    bool operator==(const Board& other) const;

    bool isPieceAt(Coord coord) const;
    Piece getPieceAt(Coord coord) const;

    Board applyMove(Move move) const;
    Board applyTurn(Turn turn) const;

    int numPiecesForColor(Piece::Color color) const;
    int scoreForColor(Piece::Color color) const;

    std::string toString() const;
private:
    std::vector<std::optional<Piece>> pieces;

    Board(std::vector<std::optional<Piece>> pieces);

    int arrayIndex(Coord coord) const;

    static std::vector<std::optional<Piece>> generateStartingPosition();
};
