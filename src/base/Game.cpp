#include "Game.hpp"
#include <stdexcept>

Game::Game(const std::string& nome, Board* b, RuleBook* r)
    : nomeJogo(nome), tabuleiro(b), regras(r), indiceJogadorDaVez(0), estadoPartida("em_andamento") {
    
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
        return;
    }
    indiceJogadorDaVez = (indiceJogadorDaVez + 1) % jogadores.size();
}

bool Game::executarJogada(int origemX, int origemY, int destinoX, int destinoY) {
    if (estadoPartida != "em_andamento") {
        return false;
    }

    if (!tabuleiro->validarCoordenadas(origemX, origemY) || 
        !tabuleiro->validarCoordenadas(destinoX, destinoY)) {
        return false;
    }

    // Exemplo de movimentação genérica: limpa origem e insere no destino
    std::string peca = tabuleiro->consultarPosicao(origemX, origemY);
    tabuleiro->removerPeca(origemX, origemY);
    tabuleiro->atualizarPosicao(destinoX, destinoY, peca);

    return true;
}

void Game::reiniciarPartida() {
    estadoPartida = "em_andamento";
    indiceJogadorDaVez = 0;
    configurarDisposicaoInicial();
}

void Game::finalizarPartida(Player* vencedor) {
    estadoPartida = "finalizada";
    if (vencedor != nullptr) {
        // Lógica de registro de vitória no jogador se necessário
    }
}