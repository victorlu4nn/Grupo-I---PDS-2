#include "Connect4.hpp" // Inclui as declarações de Connect4 e PecaConnect4, implementadas ou usadas neste arquivo.
#include <sstream> // Disponibiliza istringstream para extrair um número de uma string.

// Integracao conferida com os arquivos fornecidos:
// Board::limpar() libera as pecas e deixa as celulas como nullptr.
// Game nao possui os recursos: Connect4 libera os objetos que criou.
// Game::finalizarPartida() marca "finalizada", sem alterar a pontuacao.
// Os IDs das pecas sao 1 e 2; os indices do vetor jogadores sao 0 e 1.

Connect4::Connect4() // Inicia a definição do construtor da classe Connect4; :: identifica a classe do método.
    : Game("Connect4", new Board(LINHAS, COLUNAS), // Inicializa a base Game com o nome e um tabuleiro 6x7 alocado dinamicamente.
           new RuleBook("Dois jogadores alternam a insercao de fichas nas " // Cria o conjunto de regras e começa seu texto descritivo.
                        "colunas de 0 a 6. A ficha cai ate a ultima casa livre. " // Continua a descrição; literais de string adjacentes são unidos pelo compilador.
                        "Vence quem alinhar quatro fichas na horizontal, " // Acrescenta ao texto a condição de vitória horizontal.
                        "vertical ou diagonal. Tabuleiro cheio sem vencedor " // Acrescenta as outras direções de vitória e inicia a condição de empate.
                        "significa empate.")), // Termina a descrição e fecha as chamadas dos construtores de RuleBook e Game.
      quantidade_pecas_jogador1(0), quantidade_pecas_jogador2(0) { // Inicializa ambos os contadores em zero e abre o corpo do construtor.
    jogadores.push_back(new Player("Jogador 1", 'R')); // Cria o primeiro jogador e guarda seu ponteiro no índice 0 do vetor.
    jogadores.push_back(new Player("Jogador 2", 'A')); // Cria o segundo jogador e guarda seu ponteiro no índice 1 do vetor.
    configurarDisposicaoInicial(); // Prepara o tabuleiro vazio, os contadores, o turno e o estado inicial.
} // Encerra este bloco de instruções.

Connect4::~Connect4() { // Define o destrutor que libera os recursos criados por esta partida.
    // Board nao tem destrutor que libere as fichas: limpar vem primeiro.
    tabuleiro->limpar(); // Chama limpar pelo ponteiro: libera as fichas e deixa as casas vazias na implementação fornecida.
    delete tabuleiro; // Destrói o tabuleiro alocado com new e libera sua memória.
    delete regras; // Destrói o objeto de regras alocado com new e libera sua memória.
    for (Player* jogador : jogadores) delete jogador; // Percorre os ponteiros dos jogadores e libera cada objeto; o vetor é destruído automaticamente.
} // Encerra este bloco de instruções.

void Connect4::configurarDisposicaoInicial() { // Define a rotina que limpa a posição atual e prepara uma nova partida.
    tabuleiro->limpar(); // Chama limpar pelo ponteiro: libera as fichas e deixa as casas vazias na implementação fornecida.
    quantidade_pecas_jogador1 = 0; // Zera o número de fichas contabilizadas para o primeiro jogador.
    quantidade_pecas_jogador2 = 0; // Zera o número de fichas contabilizadas para o segundo jogador.
    indiceJogadorDaVez = 0; // Faz o jogador do índice 0 começar a partida.
    estadoPartida = "em_andamento"; // Registra que a partida está aberta para jogadas.
} // Encerra este bloco de instruções.

bool Connect4::executarJogada(const std::string acao) { // Define a conversão do comando textual em uma jogada de coluna.
    std::istringstream entrada(acao); // Cria um fluxo de leitura a partir do texto recebido.
    int coluna; // Reserva a variável que receberá a coluna; ela só será usada após uma leitura bem-sucedida.
    // Aceita espacos antes/depois, mas rejeita texto ou numeros adicionais.
    if (!(entrada >> coluna)) return false; // Tenta extrair um int; rejeita texto sem inteiro inicial ou valor fora da faixa de int.
    entrada >> std::ws; // Consome os espaços em branco restantes após o inteiro.
    if (!entrada.eof()) return false; // Rejeita conteúdo adicional, como em "2 3" ou "3x", se o fluxo não chegou ao fim.
    return soltarPeca(coluna); // Encaminha a coluna lida para a lógica do jogo e devolve seu resultado.
} // Encerra este bloco de instruções.

bool Connect4::soltarPeca(int coluna) { // Define a inserção de uma ficha do jogador da vez.
    if (coluna < 0 || coluna >= COLUNAS || // Inicia a rejeição de índices negativos ou maiores que a última coluna; || significa OU.
        estadoPartida == "finalizada" || FimdeJogo()) return false; // Também rejeita partidas já encerradas ou cujo tabuleiro indica vitória/empate.

    // Linha 0: topo. Linha 5: fundo. Procura de baixo para cima.
    int linha = LINHAS - 1; // Começa a busca na linha 5, a última linha e o fundo do tabuleiro.
    while (linha >= 0 && tabuleiro->consultarPosicao(linha, coluna) != nullptr) // Enquanto a linha for válida e a casa estiver ocupada, continua procurando acima; && significa E.
        --linha; // Subtrai uma unidade da linha para subir uma casa na coluna.
    if (linha < 0) return false; // Coluna cheia: nao altera o turno. // Se a busca ultrapassou o topo, não há espaço e a jogada falha.

    const bool primeiro = indiceJogadorDaVez == 0; // Guarda se o turno pertence ao primeiro jogador; const impede reatribuição desta variável.
    tabuleiro->atualizarPosicao(linha, coluna, // Começa a atualização da casa livre encontrada, passando a nova ficha como próximo argumento.
        new PecaConnect4(primeiro ? "R" : "A", primeiro ? 1 : 2, // Cria a ficha; o operador condição ? valor1 : valor2 escolhe símbolo e ID conforme o jogador.
                        primeiro ? EquipeConnect4::VERMELHO // Escolhe a equipe vermelha quando primeiro é true.
                                 : EquipeConnect4::AMARELO)); // Caso contrário escolhe amarela e conclui a criação e a inserção da ficha.
    if (primeiro) ++quantidade_pecas_jogador1; // Incrementa o contador do jogador 1 se ele fez a jogada.
    else ++quantidade_pecas_jogador2; // Caso contrário, incrementa o contador do jogador 2.

    if (verificar4EmLinha(linha, coluna)) { // Verifica se existe um alinhamento vencedor passando pela casa examinada.
        finalizarPartida(jogadores[indiceJogadorDaVez]); // Encerra a partida passando o jogador atual; na base fornecida isso apenas marca o estado finalizado.
    } else if (FimdeJogo()) { // Se não houve vitória nessa inserção, verifica se o tabuleiro ficou cheio.
        finalizarPartida(nullptr); // Encerra a partida sem indicar vencedor, representando empate nesta chamada.
    } else { // Executa a alternativa quando nenhuma das condições anteriores ocorreu.
        alternarTurno(); // Passa a vez para o outro jogador por meio do método herdado de Game.
    } // Encerra este bloco de instruções.
    return true; // Retorna verdadeiro: a condição verificada por esta função foi satisfeita.
} // Encerra este bloco de instruções.

bool Connect4::verificar4EmLinha(int linha, int coluna) const { // Define a consulta de alinhamento a partir de uma casa, sem alterar diretamente a partida.
    if (!tabuleiro->validarCoordenadas(linha, coluna)) return false; // Impede consultar uma casa fora dos limites do tabuleiro.
    const Piece* origem = tabuleiro->consultarPosicao(linha, coluna); // Obtém a ficha de origem; const Piece* impede modificá-la por este ponteiro.
    if (origem == nullptr) return false; // Uma casa vazia não pode fazer parte de um alinhamento de fichas.

    const int id = origem->ler_id_jogador(); // Guarda o identificador do dono para comparar as fichas vizinhas.
    const int direcoes[4][2] = {{0, 1}, {1, 0}, {1, 1}, {1, -1}}; // Define os passos (linha, coluna): horizontal, vertical e as duas diagonais.
    for (const auto& direcao : direcoes) { // Percorre as quatro direções por referência constante; auto deduz o tipo de cada par.
        int total = 1; // Conta inicialmente a própria ficha de origem.
        // Conta dos dois lados: a ultima ficha pode estar no meio da linha.
        for (int sentido : {-1, 1}) { // Examina os dois sentidos de cada direção para contar fichas de ambos os lados.
            int l = linha + sentido * direcao[0]; // Calcula a linha da primeira casa vizinha no sentido escolhido.
            int c = coluna + sentido * direcao[1]; // Calcula a coluna da primeira casa vizinha no mesmo sentido.
            while (tabuleiro->validarCoordenadas(l, c)) { // Continua a busca somente enquanto a próxima casa estiver dentro do tabuleiro.
                const Piece* peca = tabuleiro->consultarPosicao(l, c); // Lê a ficha da casa atual da busca, sem assumir que exista uma ficha ali.
                if (peca == nullptr || peca->ler_id_jogador() != id) break; // Para neste sentido ao encontrar vazio ou adversário; || evita acessar um ponteiro nulo.
                ++total; // Soma mais uma ficha consecutiva do mesmo dono.
                l += sentido * direcao[0]; // Avança a linha pelo passo da direção e do sentido atuais.
                c += sentido * direcao[1]; // Avança a coluna pelo mesmo critério.
            } // Encerra este bloco de instruções.