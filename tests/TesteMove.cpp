#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "Piece.hpp"
#include "Player.hpp"
#include "Board.hpp"
#include "Move.hpp"
#include <stdexcept>

class MovimentoQualquer: public Move {
    public:
        // Initialize a movement
        MovimentoQualquer(
            Player* p,
            int orig_x,
            int orig_y,
            int dest_x,
            int dest_y
        ): Move(p, orig_x, orig_y, dest_x, dest_y) {};

    //private:
};