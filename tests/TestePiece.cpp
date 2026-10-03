#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "Piece.hpp"
#include "Board.hpp"
#include <stdexcept>

// Subclasse concreta para testar o comportamento da classe abstrata Piece
class PecaTeste : public Piece {
public:
    PecaTeste(std::string simbolo, int id_jogador)
        : Piece(simbolo, id_jogador) {}

    // Implementação simples de movimento válido (exemplo: movimento ortogonal de 1 casa)
    bool movimento_valido(int x_orig, int y_orig, int x_dest, int y_dest, const Board& board_status) const override {
        int dx = std::abs(x_dest - x_orig);
        int dy = std::abs(y_dest - y_orig);
        return (dx + dy == 1);
    }
};

// Subclasse para testar a sobrescrita de tamanho (ex: Batalha Naval)
class NavioTeste : public Piece {
private:
    int tamanho;
public:
    NavioTeste(std::string simbolo, int id_jogador, int tam)
        : Piece(simbolo, id_jogador), tamanho(tam) {}

    bool movimento_valido(int, int, int, int, const Board&) const override {
        return false;
    }

    int ler_tamanho() const override {
        return tamanho;
    }
};

TEST_CASE("Piece - Inicialização e Getters") {
    SUBCASE("Criação de peça com valores válidos") {
        PecaTeste p("X", 1);

        CHECK(p.ler_simbolo() == "X");
        CHECK(p.ler_id_jogador() == 1);
        CHECK(p.ler_status() == PieceStatus::ATIVA);
        CHECK(p.ler_tamanho() == 1); // Tamanho padrão
    }

    SUBCASE("Validação de símbolo e ID inválidos") {
        CHECK_THROWS_AS(PecaTeste("", 1), std::invalid_argument);
        CHECK_THROWS_AS(PecaTeste("X", -1), std::invalid_argument);
    }
}

TEST_CASE("Piece - Alteração e Leitura de Status") {
    PecaTeste p("O", 2);

    CHECK(p.ler_status() == PieceStatus::ATIVA);

    p.set_status(PieceStatus::INATIVA);
    CHECK(p.ler_status() == PieceStatus::INATIVA);

    p.set_status(PieceStatus::ATIVA);
    CHECK(p.ler_status() == PieceStatus::ATIVA);
}

TEST_CASE("Piece - Sobrescrita do Tamanho da Peça") {
    NavioTeste navio("N", 1, 3);

    CHECK(navio.ler_tamanho() == 3);
}

TEST_CASE("Piece - Validação de Movimento (Polimorfismo)") {
    Board b(8, 8);
    PecaTeste p("P", 1);

    SUBCASE("Movimento válido") {
        CHECK(p.movimento_valido(0, 0, 0, 1, b) == true);
        CHECK(p.movimento_valido(2, 2, 3, 2, b) == true);
    }

    SUBCASE("Movimento inválido") {
        CHECK(p.movimento_valido(0, 0, 1, 1, b) == false);
        CHECK(p.movimento_valido(0, 0, 0, 2, b) == false);
    }
}