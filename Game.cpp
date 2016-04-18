//
// Created by Samrid on 4/11/2016.
//

#include <sstream>
#include <set>

#include "Game.h"
#include "Simple.h"
#include "Strategic.h"
#include "Food.h"
#include "Advantage.h"

using namespace std;

namespace Gaming {

    const unsigned int Game::NUM_INIT_AGENT_FACTOR = 4;
    const unsigned int Game::NUM_INIT_RESOURCE_FACTOR = 2;
    const unsigned Game::MIN_WIDTH = 3;
    const unsigned Game::MIN_HEIGHT = 3;
    const double Game::STARTING_AGENT_ENERGY = 20;
    const double Game::STARTING_RESOURCE_CAPACITY = 10;
    PositionRandomizer Game::__posRandomizer = PositionRandomizer();


    void Game::populate() {
        __numInitAgents = (__width * __height) / NUM_INIT_AGENT_FACTOR;
        __numInitResources = (__width * __height) / NUM_INIT_RESOURCE_FACTOR;

        std::default_random_engine gen;
        std::uniform_int_distribution<int> d(0, __width * __height);

        unsigned int numStrategic = __numInitAgents/2;
        unsigned int numSimple = __numInitAgents - numStrategic;
        unsigned int numAdvantages = __numInitResources/4;
        unsigned int numFoods = __numInitResources - numAdvantages;

        while (numStrategic > 0) {
            int i = d(gen);
            if (__grid[i] == nullptr) {
                Position pos(i / __width, i % __width);
                __grid[i] = new Strategic(*this, pos, STARTING_AGENT_ENERGY);
                numStrategic--;
            }
        }

        while (numSimple > 0) {
            int i = d(gen);
            if (__grid[i] == nullptr) {
                Position pos(i / __width, i % __width);
                __grid[i] = new Simple(*this, pos, STARTING_AGENT_ENERGY);
                numSimple--;
            }
        }

        while (numAdvantages > 0) {
            int i = d(gen);
            if (__grid[i] == nullptr) {
                Position pos(i / __width, i % __width);
                __grid[i] = new Advantage(*this, pos, STARTING_RESOURCE_CAPACITY);
                numAdvantages--;
            }
        }

        while (numFoods > 0) {
            int i = d(gen);
            if (__grid[i] == nullptr) {
                Position pos(i / __width, i % __width);
                __grid[i] = new Food(*this, pos, STARTING_RESOURCE_CAPACITY);
                numFoods--;
            }
        }
    }

    Game::Game() {
        __width  = MIN_WIDTH;
        __height = MIN_HEIGHT;
        __status = NOT_STARTED;
        __round = 0;
        __verbose = false;

        for(int i = 0; i < (__width*__height); ++i) {
            __grid.push_back(nullptr);
        }
    }

    Game::Game(unsigned width, unsigned height, bool manual){
        if (width < MIN_WIDTH || height < MIN_HEIGHT) {
            throw InsufficientDimensionsEx(MIN_WIDTH, MIN_HEIGHT, width, height);
        }
        __width = width;
        __height = height;
        __status = NOT_STARTED;
        __round = 0;
        __verbose = false;

        for (int i = 0; i < (__width * __height); ++i) {
            __grid.push_back(nullptr);
        }
        if (manual == false) {
            populate();
        }
    }

    Game::~Game() {
      for (int i = 0; i < __grid.size(); ++i) {
          if (__grid[i]){
              delete __grid[i];
          }
      }
    }

    unsigned int Game::getNumPieces() const {
        return getNumAgents()+getNumResources();
    }

    unsigned int Game::getNumAgents() const {
        return getNumSimple()+getNumStrategic();
    }

    unsigned int Game::getNumSimple() const {
        unsigned int numAgents = 0;

        for (auto it = __grid.begin(); it != __grid.end(); ++it) {
            Simple *simple = dynamic_cast<Simple*>(*it);
            if (simple) {
                numAgents++;
            }
        }
        return numAgents;
    }

    unsigned int Game::getNumStrategic() const {
        unsigned int numAgents = 0;

        for (auto it = __grid.begin(); it != __grid.end(); ++it) {
            Strategic *strategic = dynamic_cast<Strategic*>(*it);
            if (strategic) {
                numAgents++;
            }
        }
        return numAgents;
    }

    unsigned int Game::getNumResources() const {
        unsigned int numAgents = 0;

        for (auto it = __grid.begin(); it != __grid.end(); ++it) {
            Resource *resource = dynamic_cast<Resource*>(*it);
            if (resource) {
                numAgents++;
            }
        }
        return numAgents;
    }

    const Piece *Game::getPiece(unsigned int x, unsigned int y) const {
        if (__grid[(x * __width)+y] == nullptr) {
            throw PositionEmptyEx(x, y);
        }
        return __grid[y + (x * __width)];
    }

    void Game::addSimple(const Position &position) {
        if (position.y >= __width || position.x >= __height) {
            throw OutOfBoundsEx(__width, __height, position.x, position.y);
        }
        if (__grid[(position.x * __width)+position.y]) {
            throw PositionNonemptyEx(position.x, position.y);
        }
        __grid[(position.x * __width)+position.y] = new Simple(*this, position, STARTING_AGENT_ENERGY);
    }

    void Game::addSimple(const Position &position, double energy) {
        if (position.y >= __width || position.x >= __height) {
            throw OutOfBoundsEx(__width, __height, position.x, position.y);
        }
        if (__grid[(position.x * __width)+position.y]) {
            throw PositionNonemptyEx(position.x, position.y);
        }
        __grid[(position.x * __width)+position.y] = new Simple(*this, position, energy);
    }

    void Game::addSimple(unsigned x, unsigned y) {
        if (y >= __width || x >= __height) {
            throw OutOfBoundsEx(__width, __height, x, y);
        }
        if (__grid[(x*__width)+y]) {
            throw PositionNonemptyEx(x, y);
        }
        __grid[(x*__width)+y] = new Simple(*this, Position(x, y), STARTING_AGENT_ENERGY);
    }

    void Game::addSimple(unsigned y, unsigned x, double energy) {
        if (y >= __width || x >= __height) {
            throw OutOfBoundsEx(__width, __height, x, y);
        }
        if (__grid[(x*__width)+y]) {
            throw PositionNonemptyEx(x, y);
        }
        __grid[(x*__width)+y] = new Simple(*this, Position(x, y), energy);
    }

    void Game::addStrategic(const Position &position, Strategy *s) {
        if (position.y >= __width || position.x >= __height) {
            throw OutOfBoundsEx(__width, __height, position.x, position.y);
        }
        if (__grid[(position.x * __width)+position.y]) {
            throw PositionNonemptyEx(position.x, position.y);
        }
        __grid[(position.x * __width)+position.y] = new Strategic(*this, position, STARTING_AGENT_ENERGY, s);
    }

    void Game::addStrategic(unsigned x, unsigned y, Strategy *s) {
        if (y >= __width || x >= __height) {
            throw OutOfBoundsEx(__width, __height, x, y);
        }
        if (__grid[(x*__width)+y]) {
            throw PositionNonemptyEx(x, y);
        }
        __grid[(x*__width)+y] = new Strategic(*this, Position(x, y), STARTING_AGENT_ENERGY, s);
    }

    void Game::addFood(const Position &position) {
        if (position.y >= __width || position.x >= __height) {
            throw OutOfBoundsEx(__width, __height, position.x, position.y);
        }
        if (__grid[(position.x * __width)+position.y]) {
            throw PositionNonemptyEx(position.x, position.y);
        }
        __grid[(position.x * __width)+position.y] = new Food(*this, position, STARTING_RESOURCE_CAPACITY);
    }

    void Game::addFood(unsigned x, unsigned y) {
        if (y >= __width || x >= __height) {
            throw OutOfBoundsEx(__width, __height, x, y);
        }
        if (__grid[(x * __width)+y]) {
            throw PositionNonemptyEx(x, y);
        }
        __grid[(x * __width)+y] = new Food(*this, Position(x, y), STARTING_RESOURCE_CAPACITY);
    }

    void Game::addAdvantage(const Position &position) {
        if (position.y >= __width || position.x >= __height) {
            throw OutOfBoundsEx(__width, __height, position.x, position.y);
        }
        if (__grid[(position.x *__width)+position.y]) {
            throw PositionNonemptyEx(position.x, position.y);
        }
        __grid[(position.x *__width)+position.y] = new Advantage(*this, position, STARTING_RESOURCE_CAPACITY);
    }

    void Game::addAdvantage(unsigned x, unsigned y) {
        if (y >= __width || x >= __height) {
            throw OutOfBoundsEx(__width, __height, x, y);
        }
        if (__grid[(x *__width)+y]) {
            throw PositionNonemptyEx(x, y);
        }
        __grid[(x *__width)+y] = new Advantage(*this, Position(x, y), STARTING_RESOURCE_CAPACITY);
    }

    const Surroundings Game::getSurroundings(const Position &pos) const {
        Surroundings s;
        int x, y;

        for (int i = 0; i < 9; ++i) {
            s.array[i] = EMPTY;
        }

        for (int j = -1; j <= 1; ++j) {
            x = pos.x + j;
            for (int k = -1; k <= 1; ++k) {
                y = pos.y + k;
                if (x >= 0 && x < __height && y >= 0 && y < __width) {
                    unsigned int index = y + (x * __width);
                    if (__grid[index])
                        s.array[k + 1 + ((j + 1) * 3)] = __grid[index]->getType();
                }
                else {
                    s.array[k + 1 + ((j + 1) * 3)] = INACCESSIBLE;
                }
            }
        }

        s.array[4] = SELF;
        return s;
    }

    const ActionType Game::reachSurroundings(const Position &from, const Position &to) { // note: STAY by default
        int x = (to.x-from.x)+1;
        int y = (to.y-from.y)+1;

        int i = (x * 3) + y;

        switch (i) {
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
            default :
                break;
        }
    }

    bool Game::isLegal(const ActionType &ac, const Position &pos) const {
        int x = (pos.x);
        int y = (pos.y);

        switch (ac) {
            case NW:
                y--;
                x--;
                break;
            case N:
                x--;
                break;
            case NE:
                y++;
                x--;
                break;
            case W:
                y--;
                break;
            case E:
                y++;
                break;
            case SW:
                y--;
                x++;
                break;
            case S:
                x++;
                break;
            case SE:
                x++;
                y++;
                break;
            default :
                break;
        }
        if (x < __height && y < __width){
            return true;
        }
        else {
            return false;
        }
    }

    const Position Game::move(const Position &pos, const ActionType &ac) const { // note: assumes legal, use with isLegal()
        if (isLegal(ac, pos)) {
            int x = (pos.x);
            int y = (pos.y);

            switch (ac) {
                case NW:
                    y--;
                    x--;
                    break;
                case N:
                    x--;
                    break;
                case NE:
                    y++;
                    x--;
                    break;
                case W:
                    y--;
                    break;
                case E:
                    y++;
                    break;
                case SW:
                    y--;
                    x++;
                    break;
                case S:
                    x++;
                    break;
                case SE:
                    x++;
                    y++;
                    break;
                default :
                    break;
            }
            Position p(x, y);
            return p;
        }
    }

    void Game::round() {
        std::set<Piece*> pc;
        __status = PLAYING;

        for(int i = 0; i < __grid.size(); i++) {
            if(__grid[i] != nullptr){
                if(!(__grid[i]->isViable())){
                    __grid[i] = nullptr;
                }
            }
        }

        for (auto it = __grid.begin(); it != __grid.end(); ++it) {
            if(*it != nullptr){
                pc.insert(pc.end(), *it);
                (*it)->setTurned(false);
            }
        }

        for (auto it = pc.begin(); it != pc.end(); ++it) {
            Position p1 = (*it)->getPosition();
            ActionType a = (*it)->takeTurn(getSurroundings(p1));
            Position p2 = move(p1, a);

            if (!(*it)->getTurned()) {
                (*it)->age();
                (*it)->setTurned(true);

                if(p1.x != p2.x || p1.y != p2.y){
                        if((*it)->getPosition().x != p1.x || (*it)->getPosition().y != p1.y){
                            __grid[p2.y + (p2.x * __width)] = (*it);
                            __grid[p1.y + (p1.x * __width)] = __grid[p2.y + (p2.x * __width)];
                        }
                    else{
                        (*it)->setPosition(p2);
                        __grid[p2.y + (p2.x * __width)] = (*it);
                        __grid[p1.y + (p1.x * __width)] = nullptr;
                    }
                }
            }
        }
        __round++;
    }

    void Game::play(bool verbose) {
        __verbose = verbose;
        __status = PLAYING;
        round();

        if (getNumResources() > 0) {
            if (getNumPieces() > 0) {
                play(verbose);
            }
        }
    }

    std::ostream &operator<<(std::ostream &os, const Game &game) {

    }
}