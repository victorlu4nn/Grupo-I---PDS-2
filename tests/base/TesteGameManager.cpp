#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "GameManager.hpp"
#include "Game.hpp"
#include "Board.hpp"
#include "RuleBook.hpp"
#include <stdexcept>

// Classe concreta derivada de Game para ser usada nos testes
class MockGame : public Game {
public:
    MockGame(const std::string& nome, Board* b, RuleBook* r)
        : Game(nome, b, r) {}

    void configurarDisposicaoInicial() override {}
};

TEST_CASE("GameManager - Gerenciamento de Catálogo") {
    GameManager manager;

    SUBCASE("Catálogo inicia vazio") {
        CHECK(manager.listarJogos().empty());
    }

    SUBCASE("Registrar jogos no catálogo") {
        manager.registrarJogo("Xadrez");
        manager.registrarJogo("Damas");

        auto jogos = manager.listarJogos();
        CHECK(jogos.size() == 2);
        CHECK(jogos[0] == "Xadrez");
        CHECK(jogos[1] == "Damas");
    }

    SUBCASE("Registrar jogo com nome vazio deve lançar exceção") {
        CHECK_THROWS_AS(manager.registrarJogo(""), std::invalid_argument);
    }
}

TEST_CASE("GameManager - Inicialização de Partida") {
    GameManager manager;
    manager.registrarJogo("Xadrez");
    manager.registrarJogo("Damas");

    SUBCASE("Iniciar partida com índice válido") {
        CHECK_NOTHROW(manager.iniciarPartida(0));
        CHECK_NOTHROW(manager.iniciarPartida(1));
    }

    SUBCASE("Iniciar partida com índice inválido deve lançar exceção") {
        CHECK_THROWS_AS(manager.iniciarPartida(-1), std::out_of_range);
        CHECK_THROWS_AS(manager.iniciarPartida(2), std::out_of_range);
    }

    SUBCASE("Iniciar nova partida deve encerrar a partida anterior") {
        manager.iniciarPartida(0);
        // Garante que não há vazamento ou travamento ao sobrescrever a partida ativa
        CHECK_NOTHROW(manager.iniciarPartida(1));
    }
}

TEST_CASE("GameManager - Comandos e Limpeza") {
    GameManager manager;
    manager.registrarJogo("Xadrez");

    SUBCASE("Processar comando de encerramento sem partida ativa") {
        CHECK_NOTHROW(manager.processarComandoEncerramento());
    }

    SUBCASE("Processar comando de encerramento com partida ativa") {
        manager.iniciarPartida(0);
        CHECK_NOTHROW(manager.processarComandoEncerramento());
    }
}