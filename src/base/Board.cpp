#include "Board.hpp"
#include <stdexcept>

Board::Board(int n, int m) : linhas(n), colunas(m) {
    if (n <= 0 || m <= 0) {
        throw std::invalid_argument("Dimensões do tabuleiro devem ser maiores que zero.");
    }
    celulas.resize(linhas, std::vector<Piece*>(colunas, nullptr));
}

bool Board::validarCoordenadas(int x, int y) const {
    return (x >= 0 && x < linhas && y >= 0 && y < colunas);
}

std::string Board::consultarPosicao(int x, int y) const {
    if (!validarCoordenadas(x, y)) {
        throw std::out_of_range("Coordenadas fora dos limites do tabuleiro.");
    }
    // Retorna a representação (exemplo simples mantendo estado como string)
    return celulas[x][y] ? "Peca" : "";
}

void Board::atualizarPosicao(int x, int y, const std::string& peca) {
    if (!validarCoordenadas(x, y)) {
        throw std::out_of_range("Coordenadas fora dos limites do tabuleiro.");
    }
    // Lógica para alocar/atualizar a peça na posição (x, y)
}

void Board::removerPeca(int x, int y) {
    if (!validarCoordenadas(x, y)) {
        throw std::out_of_range("Coordenadas fora dos limites do tabuleiro.");
    }
    if (celulas[x][y] != nullptr) {
        delete celulas[x][y];
        celulas[x][y] = nullptr;
    }
}

void Board::limpar() {
    for (int i = 0; i < linhas; ++i) {
        for (int j = 0; j < colunas; ++j) {
            removerPeca(i, j);
        }
    }
}