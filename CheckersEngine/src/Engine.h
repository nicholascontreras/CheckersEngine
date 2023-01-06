#pragma once

#include <vector>
#include <functional>
#include <utility>

#include "Board.h"
#include "Turn.h"
#include "Piece.h"

class Engine {
public:
    static std::vector<Turn> getAllTurnOptions(Board board, Piece::Color color);
    static std::optional<Turn> getBestTurn(Board board, Piece::Color color, int depth);
private:
    static std::vector<Turn> getAllTurnOptionsUsingPiece(Board board, Coord pieceCoord, std::optional<Turn> prevCaptures = std::nullopt);
    static std::vector<Turn> getAllTurnOptionsUsingPieceInDir(Board board, Coord pieceCoord, Direction dir, std::optional<Turn> prevCaptures = std::nullopt);

    static bool canMoveInDirection(Board board, Coord pieceCoord, Direction dir);
    static bool canCaptureInDirection(Board board, Coord pieceCoord, Direction dir);
};
