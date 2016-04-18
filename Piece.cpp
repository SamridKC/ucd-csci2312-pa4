//
// Created by Samrid on 4/11/2016.
//

#include <sstream>

#include "Piece.h"

namespace Gaming {

    unsigned int Piece::__idGen = 1000;

    Piece::Piece(const Game &g, const Position &p) : __game(g), __position(p), __id(++__idGen)
    {
        __finished = false;
        __turned = false;
    }

    Piece::~Piece() {

    }

    std::ostream &operator<<(std::ostream &os, const Piece &piece) {
        piece.print(os);
        return os;
    }
}