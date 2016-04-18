//
// Created by Samrid on 4/11/2016.
//

#include "Resource.h"

namespace Gaming {

    const double Resource::RESOURCE_SPOIL_FACTOR = 1.2;

    Resource::Resource(const Game &g, const Position &p, double capacity) : Piece(g, p), __capacity(capacity) {

    }

    Resource::~Resource() {

    }

    double Resource::consume() {
        double consume = __capacity;
        __capacity = 0;
        finish();
        return consume;
    }

    void Resource::age() {
        if (__capacity < .0001) {
            __capacity = 0;
            finish();
        }
        else {
            __capacity /= RESOURCE_SPOIL_FACTOR;
        }
    }

    ActionType Resource::takeTurn(const Surroundings &s) const {
        return STAY;
    }

    Piece &Resource::operator*(Piece &other) {
        return *this;
    }

    Piece &Resource::interact(Agent *) {
        return *this;
    }

    Piece &Resource::interact(Resource *) {
        return *this;
    }
}