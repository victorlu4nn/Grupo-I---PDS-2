#include "RuleBook.hpp"
#include "Validacao.hpp"
#include <stdexcept>

RuleBook::RuleBook(const std::string& descricao) : descricaoRegras(descricao) {
    if (!validacao::textoVisivel(descricao) || descricao.size() > 4096)
        throw std::invalid_argument("Descricao das regras invalida.");
}
std::string RuleBook::getDescricaoRegras() const { return descricaoRegras; }
bool RuleBook::validarJogada(Board* b, int ox, int oy, int dx, int dy) const {
    if (!b) throw std::invalid_argument("Tabuleiro nulo.");
    return b->validarCoordenadas(ox, oy) && b->validarCoordenadas(dx, dy)
        && (ox != dx || oy != dy);
}
// Mantem o exemplo legado (tres simbolos iguais numa linha de um 3x3).
// Nao e uma regra universal. Os tres jogos usam suas proprias condicoes finais.
static void exigirExemplo3x3(const Board* b) {
    if (!b) throw std::invalid_argument("Tabuleiro nulo.");
    if (b->getLinhas() != 3 || b->getColunas() != 3)
        throw std::logic_error("Use a verificacao de vitoria/empate do jogo especifico.");
}
bool RuleBook::verificarVitoria(Board* b) const {
    exigirExemplo3x3(b);
    for (int l = 0; l < 3; ++l) {
        const Piece* a = b->consultarPosicao(l, 0);
        const Piece* c = b->consultarPosicao(l, 1);
        const Piece* d = b->consultarPosicao(l, 2);
        if (a && c && d && a->ler_simbolo() == c->ler_simbolo()
            && c->ler_simbolo() == d->ler_simbolo()) return true;
    }
    return false;
}
bool RuleBook::verificarEmpate(Board* b) const {
    exigirExemplo3x3(b);
    if (verificarVitoria(b)) return false;
    for (int l = 0; l < 3; ++l)
        for (int c = 0; c < 3; ++c)
            if (!b->consultarPosicao(l, c)) return false;
    return true;
}
