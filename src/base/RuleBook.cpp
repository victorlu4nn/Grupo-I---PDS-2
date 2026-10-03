#include "RuleBook.hpp"
#include <stdexcept>

RuleBook::RuleBook(const std::string& descricao) : descricaoRegras(descricao) {
    if (descricao.empty()) {
        throw std::invalid_argument("A descrição das regras não pode ser vazia.");
    }
}

std::string RuleBook::getDescricaoRegras() const {
    return descricaoRegras;
}

bool RuleBook::validarJogada(Board* tabuleiro, int origemX, int origemY, int destinoX, int destinoY) const {
    if (tabuleiro == nullptr) {
        throw std::invalid_argument("O ponteiro do tabuleiro não pode ser nulo.");
    }

    // Coordenadas devem estar dentro dos limites
    if (!tabuleiro->validarCoordenadas(origemX, origemY) || 
        !tabuleiro->validarCoordenadas(destinoX, destinoY)) {
        return false;
    }

    // Movimento para a mesma posição não é válido
    if (origemX == destinoX && origemY == destinoY) {
        return false;
    }

    return true;
}

bool RuleBook::verificarVitoria(Board* tabuleiro) const {
    if (tabuleiro == nullptr) {
        throw std::invalid_argument("O ponteiro do tabuleiro não pode ser nulo.");
    }

    // Exemplo de verificação genérica: verifica se há uma linha de 3 peças iguais
    for (int i = 0; i < 3; ++i) {
        if (tabuleiro->validarCoordenadas(i, 0) &&
            tabuleiro->validarCoordenadas(i, 1) &&
            tabuleiro->validarCoordenadas(i, 2)) {
            
            std::string p1 = tabuleiro->consultarPosicao(i, 0);
            std::string p2 = tabuleiro->consultarPosicao(i, 1);
            std::string p3 = tabuleiro->consultarPosicao(i, 2);

            if (!p1.empty() && p1 == p2 && p2 == p3) {
                return true;
            }
        }
    }

    return false;
}

bool RuleBook::verificarEmpate(Board* tabuleiro) const {
    if (tabuleiro == nullptr) {
        throw std::invalid_argument("O ponteiro do tabuleiro não pode ser nulo.");
    }

    if (verificarVitoria(tabuleiro)) {
        return false;
    }

    // Se todas as posições estiverem preenchidas sem vencedor, configura empate
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (tabuleiro->validarCoordenadas(i, j) && tabuleiro->consultarPosicao(i, j).empty()) {
                return false;
            }
        }
    }

    return true;
}