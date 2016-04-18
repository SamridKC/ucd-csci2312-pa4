//
// Created by Samrid on 4/11/2016.
//


#include <sstream>

#include "Simple.h"

namespace Gaming {

    const char Simple::SIMPLE_ID = 'S';

    Simple::Simple(const Game &g, const Position &p, double energy) : Agent(g, p, energy) {

    }

    Simple::~Simple() {

    }

    void Simple::print(std::ostream &os) const {
        os << SIMPLE_ID << __id;
    }

    ActionType Simple::takeTurn(const Surroundings &s) const {

        std::vector<int> pos;

        for (int i = 0; i < 9; i++) {
            if (s.array[i] == ADVANTAGE || s.array[i] == FOOD) {
                pos.push_back(i);
            }
        }
        if (pos.empty()) {
            for (int i = 0; i < 9; i++) {
                if (s.array[i] == EMPTY) {
                    pos.push_back(i);
                }
            }
            if (pos.empty()) {
                return STAY;
            }
        }

        if (pos.size() > 0) {

            int index = rand() % pos.size();

            switch (pos[index]) {
                case 0:
                    return NW;
                    break;
                case 1:
                    return N;
                    break;
                case 2:
                    return NE;
                    break;
                case 3:
                    return W;
                    break;
                case 4:
                    return STAY;
                    break;
                case 5:
                    return E;
                    break;
                case 6:
                    return SW;
                    break;
                case 7:
                    return S;
                    break;
                case 8:
                    return SE;
                    break;
            }
            return STAY;
        }
    }
}

