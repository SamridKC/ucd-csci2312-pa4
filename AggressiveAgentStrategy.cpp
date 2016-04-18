//
// Created by Samrid on 4/11/2016.
//

#include "Game.h"
#include "AggressiveAgentStrategy.h"

namespace Gaming {

    const double AggressiveAgentStrategy::DEFAULT_AGGRESSION_THRESHOLD = Game::STARTING_AGENT_ENERGY * 0.75;

    AggressiveAgentStrategy::AggressiveAgentStrategy(double agentEnergy) {
        __agentEnergy = agentEnergy;

    }

    AggressiveAgentStrategy::~AggressiveAgentStrategy() {

    }

    ActionType AggressiveAgentStrategy::operator()(const Surroundings &s) const {

        std::vector<int> pos;

        if (__agentEnergy >= DEFAULT_AGGRESSION_THRESHOLD) {
            for (int i = 0; i < 9; ++i) {
                if (s.array[i] == SIMPLE || s.array[i] == STRATEGIC) {
                    pos.push_back(i);
                }
            }
        }

        if (pos.empty()) {
            for (int i = 0; i < 9; ++i) {
                if (s.array[i] == ADVANTAGE) {
                    pos.push_back(i);
                }
            }
        }

        if (pos.empty()) {
            for (int i = 0; i < 9; ++i) {
                if (s.array[i] == FOOD) {
                    pos.push_back(i);
                }
            }
        }

        if (pos.empty()) {
            for (int i = 0; i < 9; ++i) {
                if (s.array[i] == EMPTY) {
                    pos.push_back(i);
                }
            }
        }

        if (pos.size() > 0) {
            int index = rand() % pos.size();

            if (pos.size() == 1) {
                index = pos[0];
            }

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
        return STAY;
    }
}
