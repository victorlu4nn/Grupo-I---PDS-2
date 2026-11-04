#include "Player.hpp"
#include <stdexcept>
#include <cctype>
#include <limits>
#include "Validacao.hpp"

Player::Player(const std::string& nome, char simbolo)
    : nome(nome), simboloPeca(simbolo), pontuacao(0) {
    if (!validacao::textoVisivel(nome) || nome.size() > 80) {
        throw std::invalid_argument("O nome do jogador não pode ser vazio.");
    }
    if (!std::isgraph(static_cast<unsigned char>(simbolo))) {
        throw std::invalid_argument("O símbolo da peça deve ser um caractere visível e válido.");
    }
}

std::string Player::getNome() const {
    return nome;
}

char Player::getSimboloPeca() const {
    return simboloPeca;
}

int Player::getPontuacao() const {
    return pontuacao;
}

void Player::adicionarPontos(int pontos) {
    if (pontos < 0) {
        throw std::invalid_argument("Não é possível adicionar uma quantidade negativa de pontos.");
    }
    if (pontos > std::numeric_limits<int>::max() - pontuacao) {
        throw std::overflow_error("A pontuacao ultrapassaria o limite de int.");
    }
    pontuacao += pontos;
}