#include "Move.hpp"
#include <stdexcept>
#include <sstream>

Move::Move(Player* j, int ox, int oy, int dx, int dy)
    : jogador(j), origemX(ox), origemY(oy), destinoX(dx), destinoY(dy), tipoCaptura("") {
    if (jogador == nullptr) {
        throw std::invalid_argument("O jogador associado ao movimento nao pode ser nulo.");
    }
}

void Move::aplicar(Board* tabuleiro) {
    if (tabuleiro == nullptr) {
        throw std::invalid_argument("O tabuleiro informado nao pode ser nulo.");
    }

    // Salva a peça que estava no destino caso haja captura
    tipoCaptura = tabuleiro->consultarPosicao(destinoX, destinoY);

    // Desloca a peça da origem para o destino
    std::string pecaOrigem = tabuleiro->consultarPosicao(origemX, origemY);
    tabuleiro->removerPeca(origemX, origemY);
    tabuleiro->atualizarPosicao(destinoX, destinoY, pecaOrigem);
}

void Move::reverter(Board* tabuleiro) {
    if (tabuleiro == nullptr) {
        throw std::invalid_argument("O tabuleiro informado nao pode ser nulo.");
    }

    // Retorna a peça movida para a posição original
    std::string pecaMovida = tabuleiro->consultarPosicao(destinoX, destinoY);
    tabuleiro->removerPeca(destinoX, destinoY);
    tabuleiro->atualizarPosicao(origemX, origemY, pecaMovida);

    // Restaura a peça capturada se houver
    if (!tipoCaptura.empty()) {
        tabuleiro->atualizarPosicao(destinoX, destinoY, tipoCaptura);
    }
}

std::string Move::toString() const {
    std::ostringstream ss;
    ss << "Movimento de " << jogador->getNome()
       << " de (" << origemX << ", " << origemY << ")"
       << " para (" << destinoX << ", " << destinoY << ")";
    return ss.str();
}