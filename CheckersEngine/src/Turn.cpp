#include "Turn.h"

#include <stdexcept>

Turn::Turn(Move move) : moves({ move }){
}

bool Turn::operator==(const Turn& other) const {
    return moves == other.moves;
}

Turn Turn::addMove(Move move) const {
    if(!move.isCapture()) {
        throw std::invalid_argument("Cannot chain non-capturing Move!");
    }

    if(!moves.back().isCapture()) {
        throw std::invalid_argument("Cannot chain move after non-capturing Move!");
    }

    std::vector<Move> newMoves = moves;
    newMoves.push_back(move);
    return Turn(newMoves);
}

bool Turn::isCapturing() const {
    return moves.front().isCapture();
}

bool Turn::startsWith(Turn other) const {
    if(other.moves.size() > moves.size()) {
        return false;
    }

    for(int i = 0; i < other.moves.size(); i++) {
        if(!(moves.at(i) == other.moves.at(i))) {
            return false;
        }
    }

    return true;
}

Turn::Turn(std::vector<Move> moves) : moves(moves) {
}
