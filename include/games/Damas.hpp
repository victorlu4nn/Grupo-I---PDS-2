/**
 * @file Damas.hpp
 * @brief Definição das classes e estruturas para o jogo de Damas no contexto de PDS 2.
 */

#ifndef DAMAS_HPP
#define DAMAS_HPP

#include <string>
#include <vector>
#include <map>

#include "Piece.hpp"
#include "Move.hpp"
#include "Game.hpp"
#include "Board.hpp"

/**
 * @struct PlayerDamas
 * @brief Estrutura que armazena o mapeamento de peças de um jogador no tabuleiro.
 */
struct PlayerDamas {
    std::map<std::string, unsigned short> mapa_pecas_no_tabuleiro;
};

/**
 * @class Damas
 * @brief Classe principal que gerencia a lógica, regras, tabuleiro e estados de uma partida de Damas.
 */
class Damas: public Game, PlayerDamas {
    private:
        // Variables that are going to determine how many pieces 
        // of each team there are on the board
        unsigned quantidade_pecas_brancas;
        unsigned quantidade_pecas_pretas;
        unsigned short contagem_lances_dama = 0;
        unsigned short contagem_finais_reduzidos = 0;

        // We also have to consider a draw position. In this case, we can compute 
        // some scenarios. We will be working with three of them:
        // 1. 20 moves with Dama, without capture or normal piece move
        // 2. Reduced endings (Regra brasileira)
        
        /**
         * @brief Atualiza e verifica a contagem de lances efetuados exclusivamente com damas.
         * @param Peca Ponteiro para a peça que realizou o movimento.
         */
        void contagem_lances_com_damas(PecaDamas* Peca);

        /**
         * @brief Verifica se a configuração atual do tabuleiro se enquadra em um final reduzido.
         * @return true se for um final reduzido, false caso contrário.
         */
        bool _verifica_final_reduzido() const;

        /**
         * @brief Controla o contador de lances para a regra de finais reduzidos.
         * @param lance_decisivo Indica se ocorreu um lance decisivo (captura ou promoção) que reinicia a contagem.
         */
        void contagem_lances_final_reduzido(bool lance_decisivo);

        /**
         * @brief Verifica se ocorreu alguma das condições oficiais de empate na partida.
         * @return true se houve empate, false caso contrário.
         */
        bool houve_empate() const;

    public:
        // Constructor and Destructor
        /**
         * @brief Constrói um novo objeto do jogo Damas, inicializando o estado inicial.
         */
        Damas(Board * damasboard, RuleBook* damasrulebook);

        /**
         * @brief Destrói o objeto Damas, liberando recursos alocados.
         */
        ~Damas();

        // Function that reads and organize actions based on the input reading
        // the main idea is to be able to read and struture multiple movements
        // when there are multiple captures
        /**
         * @brief Lê e organiza ações com base na entrada fornecida pelo usuário.
         * @details Estrutura múltiplos movimentos nos casos de capturas em cadeia.
         * @param acao String contendo a jogada inserida.
         * @return std::vector<int> Vetor estruturado com os dados do movimento.
         */
        std::vector<std::string> _lendo_movimento(const std::string acao) const;

        // Concrete function that implements abstract function in base-class 
        bool executarJogada(const std::string acao) override;

        // Another condition for a game of Damas to end, is to check if there is 
        // no other valid movement for the player. This will be a simple function
        // that inspects if there is at least one diagonal free for the player
        /**
         * @brief Verifica se o jogador atual possui pelo menos um movimento válido disponível.
         * @param celulas Matriz atual de células do tabuleiro contendo as peças.
         * @return true se houver movimentos válidos, false se o jogador estiver travado.
         */
        bool _verifica_se_ha_movimento(std::vector<std::vector<PecaDamas*>> celulas) const;

        // Verifies if the capture is valid. Takes the vector as an argument
        // and checks if every capture command is valid to validate it
        /**
         * @brief Valida se uma sequência de comandos de captura é legítima.
         * @param acoes Vetor contendo as posições e comandos da captura.
         * @return true se a captura for válida, false caso contrário.
         */
        bool _captura_valida(std::vector<int> acoes);
        
        // Stores all captured pieces into a vector for multiple taken pieces
        // due to the game capture mechanics
        /**
         * @brief Coleta e armazena todas as peças capturadas durante uma jogada múltipla.
         * @return std::vector<PecaDamas*> Vetor com ponteiros para as peças removidas.
         */
        std::vector<PecaDamas*> pecas_capturadas();

        // Initialize the damas board given a fixed structure
        /**
         * @brief Organiza e posiciona as peças nas posições iniciais padrão do tabuleiro de damas.
         */
        void organizar_tabuleiro_damas(Board* damasboard);

        /**
         * @brief Organiza e posiciona as peças nas posições iniciais padrão do tabuleiro de damas.
         */
        void configurarDisposicaoInicial() override;

        // Returns if the game has ended
        /**
         * @brief Verifica se a partida chegou ao fim (por vitória ou empate).
         * @return true se o jogo acabou, false se continua em andamento.
         */
        bool FimdeJogo() const;

        // Returns the winner
        /**
         * @brief Identifica e retorna o jogador vencedor da partida.
         * @return Player* Ponteiro para o jogador vitorioso, ou nullptr em caso de empate/jogo em curso.
         */
        Player* Vencedor() const;
};

/**
 * @class PecaDamas
 * @brief Representa uma peça individual no jogo de Damas, herdando propriedades da classe base Piece.
 */
class PecaDamas: public Piece {
    private:
        // Attribute that verifies if a piece is Dama, due to different
        // mobility mechanics
        bool eh_dama = false;

        // Update symbol when piece turns into dama
        /**
         * @brief Atualiza o símbolo visual da peça após sua promoção a Dama.
         */
        void atualizar_simbolo();

    public:
        // Constructor that initializes a piece of Damas and will work with its
        // symbol, status, boolean value eh_dama
        /**
         * @brief Constrói uma nova peça de Damas.
         * @param simb Representação em caractere/string da peça.
         * @param id_jogador Identificador numérico do dono da peça.
         */
        PecaDamas(std::string simb, int id_jogador);

        // Destructor that is called when piece is captured and excluded from the board
        /**
         * @brief Destrói a peça de Damas quando ela é capturada e removida do tabuleiro.
         */
        ~PecaDamas();

        // Getter para ler se peça eh dama
        /**
         * @brief Consulta se a peça atual possui o status de Dama.
         * @return true se for Dama, false se for pedra normal.
         */
        bool get_eh_damas() const;

        // Exclusive function that changes a piece status from normal to
        // dama when it reaches the end of the board and it is called
        /**
         * @brief Promove uma pedra comum a Dama ao atingir a última linha do tabuleiro.
         */
        void promocao_dama();

        // Function that will validate a movement for damas especifically
        // It takes bool dama as a parameter to verify if 
        /**
         * @brief Valida um movimento específico considerando as regras de mobilidade da peça.
         * @param movimento Estrutura contendo a origem e o destino do lance.
         * @return true se o movimento for válido para a peça, false caso contrário.
         */
        bool _movimento_valido(Move movimento) const;

        // Movement to be validate according to the board boundaries and apply
        // changes to the board
        bool movimento_valido(int x_orig, int y_orig, int x_dest, int y_dest, const Board& board_status) const override;

        // Literally calls movement and updates piece locals
        /**
         * @brief Executa fisicamente o movimento, atualizando a posição local da peça.
         * @param movimento Estrutura de coordenadas do movimento a ser efetuado.
         */
        void _executa_movimento(Move movimento);

        // Determines if piece reached the final opposite row of the board
        /**
         * @brief Verifica se a peça alcançou a fileira extrema oposta do tabuleiro.
         * @return true se estiver na linha de coroação, false caso contrário.
         */
        bool final_tabuleiro() const;
};

#endif