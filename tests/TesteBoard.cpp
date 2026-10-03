#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "Board.hpp"
#include <stdexcept>

TEST_CASE("Board - Inicialização e Dimensões Válidas") {
    SUBCASE("Criação de tabuleiro com dimensões válidas") {
        CHECK_NOTHROW(Board b(8, 8));
        CHECK_NOTHROW(Board b(3, 5));
    }

    SUBCASE("Tentativa de criar tabuleiro com dimensões inválidas") {
        CHECK_THROWS_AS(Board(0, 5), std::invalid_argument);
        CHECK_THROWS_AS(Board(5, 0), std::invalid_argument);
        CHECK_THROWS_AS(Board(-1, 8), std::invalid_argument);
        CHECK_THROWS_AS(Board(8, -2), std::invalid_argument);
    }
}

TEST_CASE("Board - Validação de Coordenadas") {
    Board b(5, 5);

    SUBCASE("Coordenadas dentro dos limites") {
        CHECK(b.validarCoordenadas(0, 0) == true);
        CHECK(b.validarCoordenadas(2, 3) == true);
        CHECK(b.validarCoordenadas(4, 4) == true);
    }

    SUBCASE("Coordenadas fora dos limites") {
        CHECK(b.validarCoordenadas(-1, 0) == false);
        CHECK(b.validarCoordenadas(0, -1) == false);
        CHECK(b.validarCoordenadas(5, 2) == false);
        CHECK(b.validarCoordenadas(2, 5) == false);
        CHECK(b.validarCoordenadas(10, 10) == false);
    }
}
