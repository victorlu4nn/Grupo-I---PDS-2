#include <stdexcept>

#include "Game.hpp"

Game::Game(const std::string& nome, Board* b, RuleBook* r)
    : nomeJogo(nome), tabuleiro(b), regras(r), indiceJogadorDaVez(1), estadoPartida("em_andamento") {
    
    if (tabuleiro == nullptr || regras == nullptr) {
        throw std::invalid_argument("Tabuleiro e RuleBook nao podem ser nulos.");
    }
}

Game::~Game() {
    // A classe Game apenas recebe ponteiros externos para Board e RuleBook,
    // portanto não deleta tabuleiro nem regras diretamente.
}

void Game::alternarTurno() {
    if (jogadores.empty()) {
        indiceJogadorDaVez = 0;
        throw std::invalid_argument("Nenhum jogador for cadastrado para a partida.");
    }
    indiceJogadorDaVez = (indiceJogadorDaVez + 1) % jogadores.size();
}

void Game::reiniciarPartida() {
    estadoPartida = "em_andamento";
    indiceJogadorDaVez = 0;
    configurarDisposicaoInicial();
}

void Game::finalizarPartida(Player* vencedor) {
    estadoPartida = "finalizada";
    if (vencedor != nullptr) {
        vencedor->adicionarPontos(1);
    }
}

RuleBook* Game::get_regras() const {
    return regras;
}

std::string Game::get_nomeJogo() const {
    return nomeJogo;
}

std::string Game::getEstado() const {
    return estadoPartida;
}