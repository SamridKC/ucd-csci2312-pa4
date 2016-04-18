//
// Created by Samrid on 4/11/2016.
//

#include "Agent.h"
#include "Advantage.h"

namespace Gaming {

    const double Agent::AGENT_FATIGUE_RATE = 0.3;

    Agent::Agent(const Game &g, const Position &p, double energy) : Piece(g, p) {
        __energy = energy;
    }

    Agent::~Agent() {

    }

    void Agent::age() {
        __energy -= AGENT_FATIGUE_RATE;
    }

    Piece &Agent::operator*(Piece &other) {
        Resource *__r = dynamic_cast<Resource*>(&other);
        if(__r) {
            interact(__r);
        }
        Agent *__a = dynamic_cast<Agent*>(&other);
        if(__a) {
            interact(__a);
        }
        return *this;
    }

    Piece &Agent::interact(Agent *agent) {
        if (__energy < agent->__energy) {
            agent->__energy -= __energy;
            finish();
        }
        else if (__energy == agent->__energy) {
            finish();
            agent->finish();
        }
        else if (__energy > agent->__energy) {
            __energy -= agent->__energy;
            agent->finish();
        }
        return *this;
    }

    Piece &Agent::interact(Resource *resource) {
        __energy += resource->consume();
        return *this;
    }
}
