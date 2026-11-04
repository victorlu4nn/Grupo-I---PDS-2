
#include "doctest.h"
#include "BatalhaNaval.hpp"
#include "Board.hpp"
#include "RuleBook.hpp"

#include <vector>
#include <string>

TEST_CASE("Teste de inicializacao do jogo Batalha Naval") {
    Board tabuleiro(10, 20);
    RuleBook regras("Regras de teste");

    BatalhaNaval jogo(&tabuleiro, &regras);

    CHECK_FALSE(jogo.FimdeJogo());

    CHECK_NOTHROW(jogo.organizar_tabuleiro_batalha_naval());


}

TEST_CASE("Teste de leitura de coordenadas") {
    Board tabuleiro(10, 20);
    RuleBook regras("Regras de teste");

    BatalhaNaval jogo(&tabuleiro, &regras);

    SUBCASE("Coordenada valida") {
        std::vector<int> movimento = jogo._lendo_movimento("B5");

        REQUIRE(movimento.size() == 2);
        CHECK(movimento[0] == 4);
        CHECK(movimento[1] == 1);
    }

    SUBCASE("Coordenada com letra minuscula") {
        std::vector<int> movimento = jogo._lendo_movimento("c3");

        REQUIRE(movimento.size() == 2);
        CHECK(movimento[0] == 2);
        CHECK(movimento[1] == 2);
    }

    SUBCASE("Coordenada invalida") {
        CHECK(jogo._lendo_movimento("K5").empty());
        CHECK(jogo._lendo_movimento("A0").empty());
        CHECK(jogo._lendo_movimento("A11").empty());
        CHECK(jogo._lendo_movimento("ABC").empty());
    }


}

TEST_CASE("Teste de rejeicao de jogadas invalidas") {
    Board tabuleiro(10, 20);
    RuleBook regras("Regras de teste");

    BatalhaNaval jogo(&tabuleiro, &regras);
    jogo.organizar_tabuleiro_batalha_naval();

    CHECK_FALSE(jogo.executarJogada("K5"));
    CHECK_FALSE(jogo.executarJogada("A0"));
    CHECK_FALSE(jogo.executarJogada("ABC"));

    CHECK_FALSE(jogo.FimdeJogo());


}
