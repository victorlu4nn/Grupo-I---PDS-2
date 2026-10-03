#include "Piece.hpp"
#include <stdexcept>

Piece::Piece(std::string simbolo_entry, int id)
    : simbolo(simbolo_entry), status(PieceStatus::ATIVA), id_jogador(id) {
    if (simbolo_entry.empty()) {
        throw std::invalid_argument("O símbolo da peça não pode ser vazio.");
    }
    if (id < 0) {
        throw std::invalid_argument("O ID do jogador deve ser maior ou igual a zero.");
    }
}

Piece::~Piece() {}

std::string Piece::ler_simbolo() const {
    return simbolo;
}

PieceStatus Piece::ler_status() const {
    return status;
}

void Piece::set_status(PieceStatus novo_status) {
    status = novo_status;
}

int Piece::ler_id_jogador() const {
    return id_jogador;
}