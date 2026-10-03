#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "RuleBook.hpp"
#include "Board.hpp"
#include <stdexcept>

TEST_CASE("RuleBook - Construtor e Descrição das Regras") {
    SUBCASE("Inicialização com texto válido") {
        RuleBook rb("Regras padrão de xadrez.");
        CHECK(rb.getDescricaoRegras() == "Regras padrão de xadrez.");
    }

    SUBCASE("Tentativa de criar livro de regras com texto vazio") {
        CHECK_THROWS_AS(RuleBook(""), std::invalid_argument);
    }
}

TEST_CASE("RuleBook - Validação de Jogadas") {
    RuleBook rb("Regras de jogo");
    Board b(4, 4);

    SUBCASE("Validação com ponteiro de tabuleiro nulo deve lançar exceção") {
        CHECK_THROWS_AS(rb.validarJogada(nullptr, 0, 0, 1, 1), std::invalid_argument);
    }

    SUBCASE("Jogada com coordenadas fora dos limites do tabuleiro") {
        CHECK(rb.validarJogada(&b, -1, 0, 1, 1) == false);
        CHECK(rb.validarJogada(&b, 0, 0, 5, 5) == false);
    }

    SUBCASE("Jogada sem deslocamento (origem igual ao destino)") {
        CHECK(rb.validarJogada(&b, 1, 1, 1, 1) == false);
    }

    SUBCASE("Jogada válida em coordenadas dentro do limite") {
        b.atualizarPosicao(0, 0, "Peao");
        CHECK(rb.validarJogada(&b, 0, 0, 0, 1) == true);
    }
}

TEST_CASE("RuleBook - Verificação de Condições Finais (Vitória e Empate)") {
    RuleBook rb("Regras de jogo");
    Board b(3, 3);

    SUBCASE("Passagem de tabuleiro nulo deve lançar exceção") {
        CHECK_THROWS_AS(rb.verificarVitoria(nullptr), std::invalid_argument);
        CHECK_THROWS_AS(rb.verificarEmpate(nullptr), std::invalid_argument);
    }

    SUBCASE("Estado inicial sem vitória nem empate") {
        CHECK(rb.verificarVitoria(&b) == false);
        CHECK(rb.verificarEmpate(&b) == false);
    }

    SUBCASE("Condição de vitória por preenchimento de linha (exemplo de lógica base)") {
        b.atualizarPosicao(0, 0, "X");
        b.atualizarPosicao(0, 1, "X");
        b.atualizarPosicao(0, 2, "X");

        CHECK(rb.verificarVitoria(&b) == true);
        CHECK(rb.verificarEmpate(&b) == false);
    }

    SUBCASE("Condição de empate por tabuleiro cheio sem vencedor") {
        b.atualizarPosicao(0, 0, "X"); b.atualizarPosicao(0, 1, "O"); b.atualizarPosicao(0, 2, "X");
        b.atualizarPosicao(1, 0, "X"); b.atualizarPosicao(1, 1, "O"); b.atualizarPosicao(1, 2, "O");
        b.atualizarPosicao(2, 0, "O"); b.atualizarPosicao(2, 1, "X"); b.atualizarPosicao(2, 2, "X");

        CHECK(rb.verificarVitoria(&b) == false);
        CHECK(rb.verificarEmpate(&b) == true);
    }
}