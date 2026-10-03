#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "Game.hpp"
#include "Board.hpp"
#include "Player.hpp"
#include "RuleBook.hpp"
#include <stdexcept>

// Classe concreta derivada para permitir a instanciação e teste dos métodos concretos de Game
class TestGame : public Game {
public:
    TestGame(const std::string& nome, Board* b, RuleBook* r)
        : Game(nome, b, r) {}

    void configurarDisposicaoInicial() override {
        if (tabuleiro) {
            tabuleiro->limpar();
            tabuleiro->atualizarPosicao(0, 0, "PecaInicial");
        }
    }

    // Métodos auxiliares para inspeção durante os testes
    std::string getEstadoPartida() const { return estadoPartida; }
    size_t getIndiceJogadorDaVez() const { return indiceJogadorDaVez; }
    void adicionarJogador(Player* p) { jogadores.push_back(p); }
    size_t getQuantidadeJogadores() const { return jogadores.size(); }
};

TEST_CASE("Game - Construtor e Estado Inicial") {
    Board b(8, 8);
    RuleBook r;

    SUBCASE("Inicialização válida") {
        TestGame game("Xadrez", &b, &r);
        CHECK(game.getEstadoPartida() == "em_andamento");
        CHECK(game.getIndiceJogadorDaVez() == 0);
    }

    SUBCASE("Tentativa de criar jogo com dependências nulas") {
        CHECK_THROWS_AS(TestGame("JogoInvalido", nullptr, &r), std::invalid_argument);
        CHECK_THROWS_AS(TestGame("JogoInvalido", &b, nullptr), std::invalid_argument);
    }
}

TEST_CASE("Game - Alternância de Turnos") {
    Board b(8, 8);
    RuleBook r;
    TestGame game("Damas", &b, &r);

    Player p1("Jogador 1");
    Player p2("Jogador 2");
    Player p3("Jogador 3");

    SUBCASE("Alternar sem jogadores não altera o índice ou não causa erro") {
        CHECK(game.getIndiceJogadorDaVez() == 0);
        game.alternarTurno();
        CHECK(game.getIndiceJogadorDaVez() == 0);
    }

    SUBCASE("Alternar entre 2 jogadores em ciclo") {
        game.adicionarJogador(&p1);
        game.adicionarJogador(&p2);

        CHECK(game.getIndiceJogadorDaVez() == 0);

        game.alternarTurno();
        CHECK(game.getIndiceJogadorDaVez() == 1);

        game.alternarTurno();
        CHECK(game.getIndiceJogadorDaVez() == 0);
    }

    SUBCASE("Alternar entre 3 jogadores") {
        game.adicionarJogador(&p1);
        game.adicionarJogador(&p2);
        game.adicionarJogador(&p3);

        CHECK(game.getIndiceJogadorDaVez() == 0);
        game.alternarTurno();
        CHECK(game.getIndiceJogadorDaVez() == 1);
        game.alternarTurno();
        CHECK(game.getIndiceJogadorDaVez() == 2);
        game.alternarTurno();
        CHECK(game.getIndiceJogadorDaVez() == 0);
    }
}

TEST_CASE("Game - Execução de Jogadas") {
    Board b(8, 8);
    RuleBook r;
    TestGame game("Damas", &b, &r);

    Player p1("Brancas");
    Player p2("Pretas");
    game.adicionarJogador(&p1);
    game.adicionarJogador(&p2);

    SUBCASE("Executar jogada válida") {
        // Supondo coordenadas válidas dentro dos limites do tabuleiro
        CHECK(game.executarJogada(0, 0, 1, 1) == true);
    }

    SUBCASE("Executar jogada inválida por coordenadas fora dos limites") {
        CHECK(game.executarJogada(-1, 0, 2, 2) == false);
        CHECK(game.executarJogada(0, 0, 10, 10) == false);
    }

    SUBCASE("Não permitir jogada com partida finalizada") {
        game.finalizarPartida(&p1);
        CHECK(game.getEstadoPartida() == "finalizada");
        CHECK(game.executarJogada(0, 0, 1, 1) == false);
    }
}

TEST_CASE("Game - Reinício de Partida") {
    Board b(8, 8);
    RuleBook r;
    TestGame game("Damas", &b, &r);

    Player p1("Brancas");
    Player p2("Pretas");
    game.adicionarJogador(&p1);
    game.adicionarJogador(&p2);

    // Altera o estado do jogo
    game.alternarTurno(); // passa para o jogador 1
    game.finalizarPartida(&p1);

    // Reinicia
    game.reiniciarPartida();

    CHECK(game.getEstadoPartida() == "em_andamento");
    CHECK(game.getIndiceJogadorDaVez() == 0);
    CHECK(b.consultarPosicao(0, 0) == "PecaInicial");
}

TEST_CASE("Game - Finalização de Partida") {
    Board b(8, 8);
    RuleBook r;
    TestGame game("Xadrez", &b, &r);

    Player p1("Jogador A");
    Player p2("Jogador B");
    game.adicionarJogador(&p1);
    game.adicionarJogador(&p2);

    SUBCASE("Finalizar com um vencedor") {
        game.finalizarPartida(&p1);
        CHECK(game.getEstadoPartida() == "finalizada");
    }

    SUBCASE("Finalizar com empate (nullptr)") {
        game.finalizarPartida(nullptr);
        CHECK(game.getEstadoPartida() == "finalizada");
    }
}


