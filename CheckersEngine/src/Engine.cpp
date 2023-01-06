#include "Engine.h"

#include <algorithm>

#include "util.h"

std::vector<Turn> Engine::getAllTurnOptions(Board board, Piece::Color color) {
    std::vector<Turn> allTurnOptions;

    // Find all the possible turns
    for(int row = 0; row < Board::SIZE; row++) {
        for(int col = 0; col < Board::SIZE; col++) {
            if(board.isPieceAt({ row, col })) {
                if(board.getPieceAt({ row, col }).color == color) {
                    appendVector(allTurnOptions, getAllTurnOptionsUsingPiece(board, { row, col }));
                }
            }
        }
    }

    // Determine if any of our possible turns includes a capture sequence
    bool anyCaptureOptions = false;
    for(Turn curTurnOption : allTurnOptions) {
        if(curTurnOption.isCapturing()) {
            anyCaptureOptions = true;
            break;
        }
    }

    // If a capture sequence is possible, remove all turn options that are not capture sequences
    if(anyCaptureOptions) {
        allTurnOptions.erase(std::remove_if(allTurnOptions.begin(), allTurnOptions.end(), [](Turn& t) {
            return !t.isCapturing();
        }), allTurnOptions.end());
    }

    return allTurnOptions;
}

std::optional<Turn> Engine::getBestTurn(Board board, Piece::Color color, int depth) {
    std::vector<Turn> turnOptions = getAllTurnOptions(board, color);

    if(depth == 0) {
        return turnOptions.front();
    }

    std::optional<Turn> bestTurn;
    int bestScore = 0;
    for(Turn curTurnOption : turnOptions) {
        Board curTurnOptionBoard = board.applyTurn(curTurnOption);
        std::optional<Turn> responseTurn = getBestTurn(curTurnOptionBoard, Piece::getOppositeColor(color), depth - 1);
        
        if(responseTurn.has_value()) {
            Board responseBoard = curTurnOptionBoard.applyTurn(responseTurn.value());
            int responseBoardScore = responseBoard.scoreForColor(color);

            if(!bestTurn.has_value() || responseBoardScore > bestScore) {
                bestTurn.emplace(curTurnOption);
                bestScore = responseBoardScore;
            }
        }
    }

    return bestTurn;
}

std::vector<Turn> Engine::getAllTurnOptionsUsingPiece(Board board, Coord pieceCoord, std::optional<Turn> prevCaptures) {
    Piece piece = board.getPieceAt(pieceCoord);
    std::vector<Turn> allTurnOptions;

    std::pair<Direction, Direction> forwardDirs = piece.getForwardDirections();
    appendVector(allTurnOptions, getAllTurnOptionsUsingPieceInDir(board, pieceCoord, forwardDirs.first, prevCaptures));
    appendVector(allTurnOptions, getAllTurnOptionsUsingPieceInDir(board, pieceCoord, forwardDirs.second, prevCaptures));

    if(piece.isKing) {
        std::pair<Direction, Direction> backwardsDirs = piece.getBackwardDirections();
        appendVector(allTurnOptions, getAllTurnOptionsUsingPieceInDir(board, pieceCoord, backwardsDirs.first, prevCaptures));
        appendVector(allTurnOptions, getAllTurnOptionsUsingPieceInDir(board, pieceCoord, backwardsDirs.second, prevCaptures));
    }

    return allTurnOptions;
}

std::vector<Turn> Engine::getAllTurnOptionsUsingPieceInDir(Board board, Coord pieceCoord, Direction dir, std::optional<Turn> prevCaptures) {
    if(canCaptureInDirection(board, pieceCoord, dir)) {
        Move m = { pieceCoord, pieceCoord.translate(dir).translate(dir) };
        Turn t = prevCaptures.has_value() ? prevCaptures.value().addMove(m) : Turn(m);

        std::vector<Turn> additionalCaptureTurnOptions = getAllTurnOptionsUsingPiece(board.applyMove(m), pieceCoord.translate(dir).translate(dir), t);
        if(!additionalCaptureTurnOptions.empty()) {
            return additionalCaptureTurnOptions;
        } else {
            return { t };
        }
    } else if(!prevCaptures.has_value() && canMoveInDirection(board, pieceCoord, dir)) {
        return { Turn({ pieceCoord, pieceCoord.translate(dir) }) };
    } else {
        return std::vector<Turn>();
    }
}

bool Engine::canMoveInDirection(Board board, Coord pieceCoord, Direction dir) {
    if(pieceCoord.translate(dir).isOnBoard()) {
        if(!board.isPieceAt(pieceCoord.translate(dir))) {
            return true;
        }
    }
    return false;
}

bool Engine::canCaptureInDirection(Board board, Coord pieceCoord, Direction dir) {
    if(pieceCoord.translate(dir).isOnBoard()) {
        if(board.isPieceAt(pieceCoord.translate(dir))) {
            if(board.getPieceAt(pieceCoord.translate(dir)).color != board.getPieceAt(pieceCoord).color) {
                if(pieceCoord.translate(dir).translate(dir).isOnBoard()) {
                    if(!board.isPieceAt(pieceCoord.translate(dir).translate(dir))) {
                        return true;
                    }
                }
            }
        }
    }

    return false;
}
