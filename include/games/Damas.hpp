#ifndef DAMAS_HPP
#define DAMAS_HPP
#include <map>
#include <string>
#include <vector>
#include "Game.hpp"
#include "Move.hpp"

struct PlayerDamas { std::map<std::string, unsigned short> mapa_pecas_no_tabuleiro; };
struct Movimento_Valido { bool eh_valido = false; std::vector<int> coords; };
class PecaDamas;

// Variante 8x8: pedras andam para frente e capturam nos dois sentidos;
// damas percorrem diagonais. Captura obrigatoria pela maior sequencia.
// Empate simplificado: 20 lances de cada lado so com damas, sem captura.
// Nao implementa os demais criterios de empate de torneios.
class Damas : public Game, public PlayerDamas {
private:
    unsigned quantidade_pecas_brancas = 0;
    unsigned quantidade_pecas_pretas = 0;
    unsigned contagem_lances_dama = 0;
public:
    Damas();
    Damas(Board* damasboard, RuleBook* damasrulebook);
    ~Damas() override;
    // a1 e o canto inferior esquerdo. Retorna pares "linha coluna" sem espaco,
    // ambos entre 0 e 7; vetor vazio indica erro de sintaxe.
    std::vector<std::string> _lendo_movimento(const std::string acao) const;
    bool executarJogada(const std::string acao) override;
    bool _verifica_se_ha_movimento(std::vector<std::vector<PecaDamas*>> celulas) const;
    bool _captura_valida(Movimento_Valido acoes, PecaDamas* peca,
        std::vector<PecaDamas*>& capturadas, std::vector<int>& posicoes);
    void organizar_tabuleiro_damas(Board* damasboard);
    void configurarDisposicaoInicial() override;
    bool FimdeJogo() const;
    Player* Vencedor() const;
    void exibir(std::ostream& saida) const override;
};
class PecaDamas : public Piece {
private:
    bool eh_dama = false;
    void atualizar_simbolo();
public:
    PecaDamas(std::string simb, int id_jogador);
    ~PecaDamas() override;
    bool get_eh_damas() const;
    void promocao_dama();
    // Retorno alocado: o chamador deve assumir com unique_ptr<Movimento_Valido>.
    Movimento_Valido* _movimento_valido(std::vector<std::string> movimento, Board* b) const;
    bool movimento_valido(int ox, int oy, int dx, int dy, const Board& b) const override;
    // A API antiga nao recebia tabuleiro/linha, impossibilitando estas consultas.
    void _executa_movimento(Move& movimento, Board& b);
    bool final_tabuleiro(int linha) const;
};
#endif
