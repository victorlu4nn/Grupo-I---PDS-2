#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "Player.hpp"
#include <stdexcept>

TEST_CASE("Player - Construtor e Getters") {
    SUBCASE("Criação de jogador com dados válidos") {
        Player p("Alice", 'X');

        CHECK(p.getNome() == "Alice");
        CHECK(p.getSimboloPeca() == 'X');
        CHECK(p.getPontuacao() == 0); // Pontuação inicial deve ser zero
    }

    SUBCASE("Tentativa de criar jogador com nome vazio") {
        CHECK_THROWS_AS(Player("", 'O'), std::invalid_argument);
    }

    SUBCASE("Tentativa de criar jogador com caractere de controle/invisível") {
        CHECK_THROWS_AS(Player("Bob", '\0'), std::invalid_argument);
        CHECK_THROWS_AS(Player("Bob", ' '), std::invalid_argument);
    }
}

TEST_CASE("Player - Gerenciamento de Pontuação") {
    Player p("Carlos", 'C');

    SUBCASE("Adicionar pontos positivos") {
        p.adicionarPontos(10);
        CHECK(p.getPontuacao() == 10);

        p.adicionarPontos(5);
        CHECK(p.getPontuacao() == 15);
    }

    SUBCASE("Adicionar zero pontos") {
        p.adicionarPontos(0);
        CHECK(p.getPontuacao() == 0);
    }

    SUBCASE("Tentativa de adicionar pontos negativos deve lançar exceção") {
        CHECK_THROWS_AS(p.adicionarPontos(-5), std::invalid_argument);
        CHECK(p.getPontuacao() == 0); // Garantir que a pontuação não foi alterada
    }
}