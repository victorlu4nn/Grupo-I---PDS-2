#include "Player.hpp"
#include <stdexcept>
#include <cctype>
#include <limits>
#include "Validacao.hpp"

Player::Player(const std::string& nome, std::string simbolo)
    : nome(nome), simboloPeca(simbolo), pontuacao(0) {
    if (!validacao::textoVisivel(nome) || nome.size() > 80) {
        throw std::invalid_argument("O nome do jogador não pode ser vazio.");
    }
    if (!validacao::textoVisivel(simbolo) || simbolo.size() > 32) {
        throw std::invalid_argument("O símbolo da peça do jogador não pode ser vazio nem conter caracteres de controle.");
    }
}

std::string Player::getNome() const {
    return nome;
}

std::string Player::getSimboloPeca() const {
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