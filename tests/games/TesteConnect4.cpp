#include "doctest.h" // Inclui as macros de teste CHECK, REQUIRE, CHECK_FALSE e CHECK_NOTHROW.
#include "Connect4.hpp" // Inclui as declarações de Connect4 e PecaConnect4, implementadas ou usadas neste arquivo.
#include <initializer_list> // Permite receber sequências como {0, 1, 2} em std::initializer_list.
#include <limits> // Permite consultar os valores mínimo e máximo representáveis por int.
#include <type_traits> // Disponibiliza verificações de tipos realizadas durante a compilação.

// O main do doctest deve existir em um unico arquivo do projeto.
// Para usar este arquivo sozinho, compile com
// -DDOCTEST_CONFIG_IMPLEMENT_WITH_MAIN (apenas nesta unidade de testes).
static_assert(!std::is_abstract<Connect4>::value, // Exige em tempo de compilação que Connect4 implemente todos os métodos virtuais puros.
              "Connect4 deve implementar executarJogada"); // Define a mensagem exibida se a verificação de classe concreta falhar.

// Acesso de leitura ao estado protegido, sem modificar a interface do jogo.
class Connect4Teste : public Connect4 { // Cria uma subclasse auxiliar que expõe consultas aos membros protegidos herdados de Game.
public: // Os membros seguintes podem ser usados por código externo à classe.
    Piece* peca(int linha, int coluna) const { // Declara uma consulta de ficha para os testes; o ponteiro retornado ainda permite alterar a ficha.
        return tabuleiro->consultarPosicao(linha, coluna); // Devolve o ponteiro da casa consultada para verificar a posição das fichas.
    } // Encerra este bloco de instruções.
    int turno() const { return indiceJogadorDaVez; } // Expõe o índice do jogador da vez sem modificar o turno.
    Player* jogador(int indice) const { return jogadores.at(indice); } // Devolve o jogador pelo índice; at lança exceção se o índice não existir.
}; // Encerra a declaração da classe ou a lista de inicialização acima.

static void jogar(Connect4& jogo, std::initializer_list<int> colunas) { // Define uma função auxiliar restrita a este arquivo; recebe o jogo por referência e uma lista de colunas.
    for (int coluna : colunas) REQUIRE(jogo.soltarPeca(coluna)); // Executa a sequência e interrompe o caso de teste se alguma jogada necessária falhar.
} // Encerra este bloco de instruções.

TEST_CASE("Connect4 - propriedades das fichas") { // Registra no doctest um caso independente: Connect4 - propriedades das fichas.
    PecaConnect4 vermelha("R", 1, EquipeConnect4::VERMELHO); // Cria uma ficha vermelha local com símbolo R e dono de ID 1.
    PecaConnect4 amarela("A", 2, EquipeConnect4::AMARELO); // Cria uma ficha amarela local com símbolo A e dono de ID 2.
    CHECK(vermelha.ler_simbolo() == "R"); // Confere que o símbolo da ficha seja "R".
    CHECK(vermelha.ler_id_jogador() == 1); // Confere que a ficha pertença ao jogador de ID 1.
    CHECK(vermelha.getEquipe() == EquipeConnect4::VERMELHO); // Confere que a equipe da ficha seja EquipeConnect4::VERMELHO.
    CHECK(amarela.ler_simbolo() == "A"); // Confere que o símbolo da ficha seja "A".
    CHECK(amarela.ler_id_jogador() == 2); // Confere que a ficha pertença ao jogador de ID 2.
    CHECK(amarela.getEquipe() == EquipeConnect4::AMARELO); // Confere que a equipe da ficha seja EquipeConnect4::AMARELO.
    Board tabuleiro(6, 7); // Cria um tabuleiro local de seis linhas e sete colunas para os testes de movimento.
    CHECK_FALSE(vermelha.movimento_valido(5, 0, 4, 0, tabuleiro)); // Confere que seja FALSO que a ficha aceite o deslocamento indicado; no Connect4 esse resultado deve ser falso.
    CHECK_FALSE(vermelha.movimento_valido(5, 0, 5, 0, tabuleiro)); // Confere que seja FALSO que a ficha aceite o deslocamento indicado; no Connect4 esse resultado deve ser falso.
    CHECK_FALSE(amarela.movimento_valido(-1, 0, 6, 7, tabuleiro)); // Confere que seja FALSO que a ficha aceite o deslocamento indicado; no Connect4 esse resultado deve ser falso.
} // Encerra este bloco de instruções.

TEST_CASE("Connect4 - inicializacao") { // Registra no doctest um caso independente: Connect4 - inicializacao.
    Connect4Teste jogo; // Cria uma partida independente para este cenário; ela será destruída automaticamente ao sair do escopo.
    CHECK_FALSE(jogo.FimdeJogo()); // Confere que seja FALSO que a consulta indique término da partida.
    CHECK(jogo.Vencedor() == nullptr); // Confere que não exista jogador vencedor.
    CHECK(jogo.getEstado() == "em_andamento"); // Confere que o estado da partida seja "em_andamento".
    CHECK(jogo.turno() == 0); // Confere que o turno seja 0 (0: primeiro jogador; 1: segundo jogador).
    REQUIRE(jogo.get_regras() != nullptr); // Exige, interrompendo o caso se falhar, que o objeto de regras exista antes de acessar seus métodos.
    CHECK_FALSE(jogo.get_regras()->getDescricaoRegras().empty()); // Confere que seja FALSO que a descrição das regras esteja vazia.
    for (int l = 0; l < 6; ++l) // Percorre todas as linhas para verificar o estado do tabuleiro.
        for (int c = 0; c < 7; ++c) CHECK(jogo.peca(l, c) == nullptr); // Percorre as colunas e confere que cada casa está vazia; CHECK registra falha e continua o teste.
} // Encerra este bloco de instruções.

TEST_CASE("Connect4 - gravidade e alternancia de turnos") { // Registra no doctest um caso independente: Connect4 - gravidade e alternancia de turnos.
    Connect4Teste jogo; // Cria uma partida independente para este cenário; ela será destruída automaticamente ao sair do escopo.
    REQUIRE(jogo.soltarPeca(3)); // Exige, interrompendo o caso se falhar, que a inserção na coluna 3 seja aceita.
    REQUIRE(jogo.peca(5, 3) != nullptr); // Exige, interrompendo o caso se falhar, que o ponteiro consultado seja válido antes de acessar a ficha.
    CHECK(jogo.peca(5, 3)->ler_id_jogador() == 1); // Confere que a ficha pertença ao jogador de ID 1.
    CHECK(jogo.turno() == 1); // Confere que o turno seja 1 (0: primeiro jogador; 1: segundo jogador).
    REQUIRE(jogo.soltarPeca(3)); // Exige, interrompendo o caso se falhar, que a inserção na coluna 3 seja aceita.
    REQUIRE(jogo.peca(4, 3) != nullptr); // Exige, interrompendo o caso se falhar, que o ponteiro consultado seja válido antes de acessar a ficha.
    CHECK(jogo.peca(4, 3)->ler_id_jogador() == 2); // Confere que a ficha pertença ao jogador de ID 2.
    CHECK(jogo.peca(3, 3) == nullptr); // Confere que a casa consultada esteja vazia.
    CHECK(jogo.turno() == 0); // Confere que o turno seja 0 (0: primeiro jogador; 1: segundo jogador).
} // Encerra este bloco de instruções.

TEST_CASE("Connect4 - colunas invalidas preservam estado e turno") { // Registra no doctest um caso independente: Connect4 - colunas invalidas preservam estado e turno.
    Connect4Teste jogo; // Cria uma partida independente para este cenário; ela será destruída automaticamente ao sair do escopo.
    for (int coluna : {-1, 7, std::numeric_limits<int>::min(), // Começa uma lista de colunas inválidas, incluindo o menor int representável.
                       std::numeric_limits<int>::max()}) { // Completa a lista com o maior int e abre o laço de validação de limites.
        CHECK_FALSE(jogo.soltarPeca(coluna)); // Confere que seja FALSO que a inserção na coluna coluna seja aceita.
        CHECK(jogo.turno() == 0); // Confere que o turno seja 0 (0: primeiro jogador; 1: segundo jogador).
        CHECK_FALSE(jogo.FimdeJogo()); // Confere que seja FALSO que a consulta indique término da partida.
    } // Encerra este bloco de instruções.
    for (int l = 0; l < 6; ++l) // Percorre todas as linhas para verificar o estado do tabuleiro.
        for (int c = 0; c < 7; ++c) CHECK(jogo.peca(l, c) == nullptr); // Percorre as colunas e confere que cada casa está vazia; CHECK registra falha e continua o teste.
} // Encerra este bloco de instruções.

TEST_CASE("Connect4 - coluna cheia") { // Registra no doctest um caso independente: Connect4 - coluna cheia.
    Connect4Teste jogo; // Cria uma partida independente para este cenário; ela será destruída automaticamente ao sair do escopo.
    jogar(jogo, {0, 0, 0, 0, 0, 0}); // Executa, da esquerda para a direita, as colunas listadas; os jogadores se alternam a cada inserção válida.
    CHECK_FALSE(jogo.FimdeJogo()); // Confere que seja FALSO que a consulta indique término da partida.
    Piece* topo = jogo.peca(0, 0); // Guarda o endereço da ficha do topo para verificar se uma tentativa inválida a preserva.
    REQUIRE(topo != nullptr); // Exige, interrompendo o caso se falhar, que o ponteiro consultado seja válido antes de acessar a ficha.
    CHECK_FALSE(jogo.soltarPeca(0)); // Confere que seja FALSO que a inserção na coluna 0 seja aceita.
    CHECK(jogo.peca(0, 0) == topo); // Confere que a ficha do topo seja a mesma, sem substituição após a tentativa na coluna cheia.
    CHECK(jogo.turno() == 0); // Confere que o turno seja 0 (0: primeiro jogador; 1: segundo jogador).
    REQUIRE(jogo.soltarPeca(6)); // Exige, interrompendo o caso se falhar, que a inserção na coluna 6 seja aceita.
    CHECK(jogo.peca(5, 6)->ler_id_jogador() == 1); // Confere que a ficha pertença ao jogador de ID 1.
} // Encerra este bloco de instruções.

TEST_CASE("Connect4 - comandos textuais e chamada pela classe base") { // Registra no doctest um caso independente: Connect4 - comandos textuais e chamada pela classe base.
    Connect4Teste jogo; // Cria uma partida independente para este cenário; ela será destruída automaticamente ao sair do escopo.
    Game& base = jogo; // Cria uma referência Game para a mesma partida, permitindo testar o despacho virtual.
    for (const char* comando : {"", " ", "abc", "3x", "2 3", "1.5", // Percorre textos inválidos: vazio, espaços, letras, sufixo, dois números e decimal.
                                "999999999999999999999", "-1", "7"}) { // Inclui um número grande demais para int e duas colunas fora da faixa, completando a lista.