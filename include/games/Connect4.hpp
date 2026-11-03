#ifndef CONNECT4_HPP // Verifica se este cabeçalho ainda não foi incluído nesta unidade de compilação.
#define CONNECT4_HPP // Define a marca de inclusão para evitar declarações repetidas do cabeçalho.

#include <string> // Disponibiliza std::string, usada para símbolos, comandos e textos.
#include <vector> // Disponibiliza std::vector, um contêiner de tamanho variável.
#include "Piece.hpp" // Inclui a classe base Piece, da qual as fichas do Connect4 herdam.
#include "Move.hpp" // Inclui as declarações de Move; este jogo não usa deslocamentos de peças.
#include "Game.hpp" // Inclui a classe base Game e a interface comum das partidas.

class PecaConnect4; // Declara antecipadamente que PecaConnect4 é uma classe, sem definir seus membros ainda.

class Connect4 : public Game { // Define Connect4 como uma especialização pública de Game, herdando sua interface.
private: // Os membros seguintes só podem ser acessados diretamente pela própria classe e seus amigos.
    static const int LINHAS = 6; // Define seis linhas; static faz a constante pertencer à classe, não a cada objeto.
    static const int COLUNAS = 7; // Define sete colunas, cujos índices válidos vão de 0 a 6.
    unsigned quantidade_pecas_jogador1; // Declara o contador sem sinal das fichas do primeiro jogador.
    unsigned quantidade_pecas_jogador2; // Declara o contador sem sinal das fichas do segundo jogador.

public: // Os membros seguintes podem ser usados por código externo à classe.
    Connect4(); // Declara o construtor, chamado ao criar uma partida.
    ~Connect4() override; // Declara o destrutor; override confirma que sobrescreve o destrutor virtual de Game.
    // Evita copias rasas e liberacao duplicada dos recursos da partida.
    Connect4(const Connect4&) = delete; // Proíbe construir uma partida copiando outra, evitando compartilhar ponteiros proprietários.
    Connect4& operator=(const Connect4&) = delete; // Proíbe atribuir uma partida a outra e duplicar a responsabilidade por liberar os recursos.
    void configurarDisposicaoInicial() override; // Declara a reinicialização específica de Connect4, sobrescrevendo a função de Game.
    // Declaracao necessaria porque executarJogada e virtual pura em Game.
    // O comando deve conter uma coluna de 0 a 6 (exemplo: "3").
    bool executarJogada(const std::string acao) override; // Declara a interpretação do comando textual; retorna se a jogada foi realizada.
    bool soltarPeca(int coluna); // Declara a inserção com gravidade na coluna indicada; true representa sucesso.
    bool FimdeJogo() const; // Declara a consulta de vitória ou empate; const impede alterar diretamente os membros do objeto.
    Player* Vencedor() const; // Declara a consulta que devolve um jogador existente ou nullptr quando não há vencedor.
    bool verificar4EmLinha(int linha, int coluna) const; // Declara a busca de quatro fichas alinhadas que passem pela posição informada.
}; // Encerra a declaração da classe ou a lista de inicialização acima.

enum class EquipeConnect4 { VERMELHO, AMARELO }; // Define duas equipes em um enum com escopo próprio: EquipeConnect4::VERMELHO e ::AMARELO.

class PecaConnect4 : public Piece { // Define a ficha como especialização pública de Piece, herdando símbolo e identificação do dono.
private: // Os membros seguintes só podem ser acessados diretamente pela própria classe e seus amigos.
    EquipeConnect4 equipe; // Guarda a cor/equipe desta ficha.
public: // Os membros seguintes podem ser usados por código externo à classe.
    PecaConnect4(std::string simbolo, int id_jogador, EquipeConnect4 eq); // Declara o construtor com símbolo, identificador do dono e equipe.
    ~PecaConnect4() override; // Declara o destrutor virtual da ficha, permitindo destruição pela interface Piece.
    bool movimento_valido(int x_orig, int y_orig, int x_dest, int y_dest, // Começa a declaração da validação de deslocamento, recebendo coordenadas de origem e destino.
                         const Board& board_status) const override; // Recebe o tabuleiro por referência constante e sobrescreve o método virtual sem modificar a ficha.
    EquipeConnect4 getEquipe() const; // Declara a consulta da equipe, sem alterar diretamente o objeto.
}; // Encerra a declaração da classe ou a lista de inicialização acima.

#endif // Encerra a proteção contra inclusões repetidas.
