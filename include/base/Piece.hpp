#ifndef PIECE_HPP
#define PIECE_HPP

#include <string>
#include <vector>

class Board;

/**
 * @enum PieceStatus
 * @brief Define os estados possíveis de uma peça em jogo de forma segura.
 */
enum class PieceStatus {
    ATIVA,
    INATIVA
};

class Piece {
    private:
        std::string simbolo; ///< Símbolo visual da peça no terminal (ex: "X", "O", "♔")
        PieceStatus status;  ///< Estado atual da peça
        int id_jogador;      ///< Identificador de qual jogador é o dono desta peça

    public:
        /**
         * @brief Construtor base da peça.
         * @param simb Representação visual da peça.
         * @param id Dono da peça.
         */
        Piece(std::string simbolo_entry, int id);
        
        /**
         * @brief Destrutor virtual.
         */
        virtual ~Piece();

        /**
         * @brief Método virtual puro que delega a validação para as subclasses.
         * @param origX Coordenada X de origem.
         * @param origY Coordenada Y de origem.
         * @param destX Coordenada X de destino.
         * @param destY Coordenada Y de destino.
         * @param tabuleiro Referência constante ao tabuleiro para checar obstáculos/capturas.
         * @return true se o movimento respeita as regras da peça específica.
         */
        virtual bool movimento_valido(int x_orig, int y_orig, int x_dest, int y_dest, const Board& board_status) const = 0;
        
        std::string ler_simbolo() const;
        PieceStatus ler_status() const;
        void set_status(PieceStatus novo_status);
        int ler_id_jogador() const;

        /**
         * @brief Retorna o tamanho da peça (útil para jogos como Batalha Naval).
         * Subclasses como NavioPiece podem sobrescrever para retornar 2, 3, 4, etc.
         * O padrão para jogos clássicos é 1.
         */
        virtual int ler_tamanho() const { return 1; }
};

#endif