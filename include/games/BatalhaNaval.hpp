
#ifndef BATALHANAVAL_HPP
#define BATALHANAVAL_HPP

#include <string>
#include <vector>

#include "Piece.hpp"
#include "Move.hpp"
#include "Game.hpp"
#include "Board.hpp"
#include "RuleBook.hpp"
#include "Player.hpp"

/**
 * @class BatalhaNaval
 * @brief Implementa as regras e a lógica específica do jogo Batalha Naval.
 *
 * Cada jogador possui um mar de 10x10 posições. Os dois mares ficam lado a
 * lado no mesmo tabuleiro, que deve ter pelo menos 10 linhas e 20 colunas.
 *
 * O jogador 1 utiliza as colunas 0 a 9, enquanto o jogador 2 utiliza as
 * colunas 10 a 19. As coordenadas utilizadas pelos métodos auxiliares são
 * relativas ao mar de cada jogador.
 *
 * Os identificadores das peças são 1 para o jogador 1 e 2 para o jogador 2.
 */
class BatalhaNaval : public Game {
private:
    /**
     * @brief Quantidade de navios restantes do jogador 1.
     */
    unsigned quantidade_navios_jogador1;

    /**
     * @brief Quantidade de navios restantes do jogador 2.
     */
    unsigned quantidade_navios_jogador2;

public:
    /**
     * @brief Constrói uma partida de Batalha Naval.
     * @param tabuleiro Ponteiro para o tabuleiro compartilhado pelos dois mares.
     * @param regras Ponteiro para o conjunto de regras da partida.
     */
    BatalhaNaval(Board* tabuleiro, RuleBook* regras);

    /**
     * @brief Destrói a partida e libera os jogadores criados pela classe.
     */
    ~BatalhaNaval() override;

    /**
     * @brief Impede a cópia da partida para evitar a duplicação de propriedade
     * dos objetos Player.
     */
    BatalhaNaval(const BatalhaNaval&) = delete;

    /**
     * @brief Impede a atribuição entre partidas de Batalha Naval.
     */
    BatalhaNaval& operator=(const BatalhaNaval&) = delete;

    /**
     * @brief Configura a disposição inicial dos navios no tabuleiro.
     */
    void configurarDisposicaoInicial() override;

    /**
     * @brief Executa uma jogada a partir de um comando informado pelo jogador.
     * @param acao Comando contendo a coordenada atacada, como "B5".
     * @return true se a jogada foi aceita; false se o comando ou ataque for inválido.
     */
    bool executarJogada(const std::string acao) override;

    /**
     * @brief Limpa o tabuleiro e posiciona aleatoriamente a frota dos dois jogadores.
     */
    void organizar_tabuleiro_batalha_naval();

    /**
     * @brief Posiciona um navio no mar de um jogador.
     *
     * O navio é disposto horizontalmente para a direita ou verticalmente
     * para baixo. Não pode ultrapassar os limites, sobrepor outro navio
     * nem tocar outro navio na horizontal ou vertical.
     *
     * @param jogador Identificador do jogador: 1 ou 2.
     * @param linha Linha inicial do navio.
     * @param coluna Coluna inicial do navio.
     * @param tamanho Quantidade de posições ocupadas pelo navio.
     * @param horizontal true para orientação horizontal; false para vertical.
     * @return true se o navio foi posicionado; false caso contrário.
     */
    bool posicionar_navio(
        int jogador,
        int linha,
        int coluna,
        int tamanho,
        bool horizontal
    );

    /**
     * @brief Converte o comando do jogador em coordenadas do tabuleiro.
     *
     * Aceita coordenadas como "B5", utilizando letras de A a J e linhas
     * de 1 a 10. As coordenadas retornadas começam em zero.
     *
     * @param acao Comando contendo a coordenada atacada.
     * @return Vetor contendo linha e coluna, nessa ordem, ou vetor vazio
     * caso o comando seja inválido.
     */
    std::vector<int> _lendo_movimento(const std::string acao) const;

    /**
     * @brief Verifica se uma posição do mar adversário pode ser atacada.
     * @param linha Linha da posição atacada, começando em zero.
     * @param coluna Coluna da posição atacada, começando em zero.
     * @return true se a posição estiver dentro dos limites e ainda não tiver
     * sido atacada; false caso contrário.
     */
    bool _movimento_valido(int linha, int coluna) const;

    /**
     * @brief Executa o ataque à posição indicada do mar adversário.
     * @param linha Linha da posição atacada, começando em zero.
     * @param coluna Coluna da posição atacada, começando em zero.
     */
    void _executa_movimento(int linha, int coluna);

    /**
     * @brief Consulta a quantidade de navios restantes de um jogador.
     * @param jogador Identificador do jogador: 1 ou 2.
     * @return Quantidade de navios restantes, ou zero para um identificador inválido.
     */
    unsigned navios_restantes(int jogador) const;

    /**
     * @brief Verifica se a partida foi finalizada.
     * @return true se a partida terminou; false caso contrário.
     */
    bool FimdeJogo() const;

    /**
     * @brief Retorna o jogador vencedor da partida.
     * @return Ponteiro para o jogador vencedor ou nullptr se não houver vencedor.
     */
    Player* Vencedor() const;
};

/**
 * @class PecaBatalhaNaval
 * @brief Representa uma posição do mar, contendo um navio ou uma marca de água.
 *
 * A peça registra se representa um navio e se já foi atingida durante a partida.
 */
class PecaBatalhaNaval : public Piece {
private:
    /**
     * @brief Indica se a posição contém um navio.
     */
    bool eh_navio;

    /**
     * @brief Indica se a posição já foi atingida.
     */
    bool foi_atingido;

public:
    /**
     * @brief Constrói uma peça ou marca de água.
     * @param simb Símbolo visual da posição.
     * @param id_jogador Identificador do jogador proprietário.
     * @param navio true se a posição contém um navio; false se representa água.
     */
    PecaBatalhaNaval(std::string simb, int id_jogador, bool navio = true);

    /**
     * @brief Destrói a peça.
     */
    ~PecaBatalhaNaval() override;

    /**
     * @brief Verifica se a posição contém um navio.
     * @return true se contém um navio; false caso contrário.
     */
    bool get_eh_navio() const;

    /**
     * @brief Verifica se a posição já foi atingida.
     * @return true se já foi atingida; false caso contrário.
     */
    bool get_foi_atingido() const;

    /**
     * @brief Marca a posição como atingida.
     */
    void atingir();

    /**
     * @brief Implementa a validação de movimento herdada de Piece.
     *
     * As peças da Batalha Naval não se deslocam depois de posicionadas.
     *
     * @param x_orig Linha de origem.
     * @param y_orig Coluna de origem.
     * @param x_dest Linha de destino.
     * @param y_dest Coluna de destino.
     * @param board_status Estado do tabuleiro utilizado na validação.
     * @return false, pois as peças não realizam movimentos.
     */
    bool movimento_valido(
        int x_orig,
        int y_orig,
        int x_dest,
        int y_dest,
        const Board& board_status
    ) const override;
};

#endif
