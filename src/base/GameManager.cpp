#include "GameManager.hpp"
#include "Board.hpp"
#include "RuleBook.hpp"
#include <stdexcept>
#include <iostream>

// Classe concreta genérica interna para inicialização padrão de partidas
class GameGenerico : public Game {
public:
    GameGenerico(const std::string& nome, Board* b, RuleBook* r)
        : Game(nome, b, r) {}

    void configurarDisposicaoInicial() override {
        if (tabuleiro) {
            tabuleiro->limpar();
        }
    }
    // Basic verification just for implementation
    bool executarJogada(const std::string acao) {
        if (acao == "jogada") {
            return true;
        }
    }
};

GameManager::GameManager() : partidaAtual(nullptr) {}

GameManager::~GameManager() {
    if (partidaAtual != nullptr) {
        delete partidaAtual;
        partidaAtual = nullptr;
    }
}

void GameManager::registrarJogo(const std::string& nomeJogo) {
    if (nomeJogo.empty()) {
        throw std::invalid_argument("O nome do jogo nao pode ser vazio.");
    }
    catalogoJogos.push_back(nomeJogo);
}

std::vector<std::string> GameManager::listarJogos() const {
    return catalogoJogos;
}

void GameManager::iniciarPartida(int indice) {
    if (indice < 0 || static_cast<size_t>(indice) >= catalogoJogos.size()) {
        throw std::out_of_range("Indice de jogo invalido no catalogo.");
    }

    // Limpa a partida anterior caso exista
    if (partidaAtual != nullptr) {
        delete partidaAtual;
        partidaAtual = nullptr;
    }

    // Instancia os componentes necessários para a nova partida
    Board* b = new Board(8, 8);
    RuleBook* r = new RuleBook("regra: ...");

    partidaAtual = new GameGenerico(catalogoJogos[indice], b, r);
    partidaAtual->configurarDisposicaoInicial();
}

void GameManager::executarLoopPartida() {
    if (partidaAtual == nullptr) {
        return;
    }

    while (partidaAtual->getEstado() != "finalizada" && partidaAtual != nullptr) {
        std::cout << "Insira um comando ou jogada: " << std::endl;

        std::string line;
        std::getline(std::cin, line);
        if (line == "regras") {
            getDescricaoRegras();
        } else if (line == "sair partida") {
            partidaAtual->reiniciarPartida();
        } else if (line == "sair programa") {
            std::cout << "Encerrando programa..." << std::endl;
            exit(0);
        } else {
            partidaAtual->executarJogada(line);
        }
    }
}

void GameManager::processarComandoEncerramento() {
    if (partidaAtual != nullptr) {
        delete partidaAtual;
        partidaAtual = nullptr;
    }
}

// Function that returns the rules to the specific game 
void GameManager::getDescricaoRegras() const {
    RuleBook* regras_partida = partidaAtual->get_regras();
    std::cout << "Regras de " << partidaAtual->get_nomeJogo() << " :" << std::endl;
    std::cout << regras_partida->getDescricaoRegras() << std::endl;
}