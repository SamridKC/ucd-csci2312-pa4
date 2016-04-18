//
// Created by Samrid on 4/11/2016.
//

#include "Advantage.h"

namespace Gaming {

    const char Advantage::ADVANTAGE_ID = 'D';

    const double Advantage::ADVANTAGE_MULT_FACTOR = 2.0;

    Advantage::Advantage(const Game &g, const Position &p, double capacity) : Resource(g, p, capacity) {

    }

    Advantage::~Advantage() {

    }

    void Advantage::print(std::ostream &os) const {
        os << ADVANTAGE_ID << __id;
    }

    double Advantage::getCapacity() const {
        return __capacity;
    }

    double Advantage::consume() {
        double consume = __capacity * ADVANTAGE_MULT_FACTOR;
        __capacity = 0;
        finish();
        return consume;
    }
}