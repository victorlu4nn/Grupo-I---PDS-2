// Correcao de tipos para a interface atual de Board (Piece*).
// Mantem a regra de exemplo original de tres pecas; Connect4 usa sua propria
// verificacao de quatro em linha e de empate. Substitui o RuleBook.cpp original.
#include "RuleBook.hpp" // Inclui a declaração da classe RuleBook, implementada neste arquivo.
#include <stdexcept> // Disponibiliza std::invalid_argument para sinalizar argumentos inválidos.

RuleBook::RuleBook(const std::string& descricao) : descricaoRegras(descricao) { // Recebe a descrição por referência constante e a copia para o membro descricaoRegras.
    if (descricao.empty()) { // Verifica se a descrição recebida não contém caracteres.
        throw std::invalid_argument("A descrição das regras não pode ser vazia."); // Interrompe a construção lançando uma exceção que explica o argumento inválido.
    } // Encerra este bloco de instruções.
} // Encerra este bloco de instruções.

std::string RuleBook::getDescricaoRegras() const { // Define a consulta que retorna uma cópia do texto das regras.
    return descricaoRegras; // Devolve a descrição armazenada no objeto.
} // Encerra este bloco de instruções.

bool RuleBook::validarJogada(Board* tabuleiro, int origemX, int origemY, int destinoX, int destinoY) const { // Define uma validação básica de coordenadas; não verifica regras específicas de todas as peças/jogos.
    if (tabuleiro == nullptr) { // Detecta a ausência de um tabuleiro antes de acessar seus métodos.
        throw std::invalid_argument("O ponteiro do tabuleiro não pode ser nulo."); // Lança uma exceção para informar que o chamador não forneceu um tabuleiro válido.
    } // Encerra este bloco de instruções.

    // Coordenadas devem estar dentro dos limites
    if (!tabuleiro->validarCoordenadas(origemX, origemY) ||  // Inicia a rejeição da jogada caso a origem esteja fora do tabuleiro.
        !tabuleiro->validarCoordenadas(destinoX, destinoY)) { // Também rejeita se o destino estiver fora dos limites.
        return false; // Retorna falso: a condição verificada por esta função não foi satisfeita.
    } // Encerra este bloco de instruções.

    // Movimento para a mesma posição não é válido
    if (origemX == destinoX && origemY == destinoY) { // Compara as duas coordenadas para impedir deslocamento de uma casa para ela mesma.
        return false; // Retorna falso: a condição verificada por esta função não foi satisfeita.
    } // Encerra este bloco de instruções.

    return true; // Retorna verdadeiro: a condição verificada por esta função foi satisfeita.
} // Encerra este bloco de instruções.

bool RuleBook::verificarVitoria(Board* tabuleiro) const { // Define a verificação de exemplo de três símbolos iguais nas primeiras três linhas; não é a regra do Connect4.
    if (tabuleiro == nullptr) { // Detecta a ausência de um tabuleiro antes de acessar seus métodos.
        throw std::invalid_argument("O ponteiro do tabuleiro não pode ser nulo."); // Lança uma exceção para informar que o chamador não forneceu um tabuleiro válido.
    } // Encerra este bloco de instruções.

    // Exemplo de verificação genérica: verifica se há uma linha de 3 peças iguais
    for (int i = 0; i < 3; ++i) { // Percorre os índices 0, 1 e 2; esta regra genérica mantém o limite fixo do exemplo original.
        if (tabuleiro->validarCoordenadas(i, 0) && // Confere se existe a primeira casa da linha i.
            tabuleiro->validarCoordenadas(i, 1) && // Exige que a segunda casa da mesma linha também exista.
            tabuleiro->validarCoordenadas(i, 2)) { // Exige que a terceira casa exista antes de consultar as três posições.
            
            const Piece* p1 = tabuleiro->consultarPosicao(i, 0); // Lê o ponteiro da primeira ficha; casa vazia é representada por nullptr, não por texto.
            const Piece* p2 = tabuleiro->consultarPosicao(i, 1); // Lê o ponteiro da segunda ficha da linha.
            const Piece* p3 = tabuleiro->consultarPosicao(i, 2); // Lê o ponteiro da terceira ficha da linha.

            if (p1 != nullptr && p2 != nullptr && p3 != nullptr && // Exige três fichas existentes; o curto-circuito de && impede acessar ponteiros nulos.
                p1->ler_simbolo() == p2->ler_simbolo() && // Compara os símbolos da primeira e da segunda ficha.
                p2->ler_simbolo() == p3->ler_simbolo()) { // Compara os símbolos da segunda e da terceira, completando a igualdade das três.
                return true; // Retorna verdadeiro: a condição verificada por esta função foi satisfeita.
            } // Encerra este bloco de instruções.
        } // Encerra este bloco de instruções.
    } // Encerra este bloco de instruções.

    return false; // Retorna falso: a condição verificada por esta função não foi satisfeita.
} // Encerra este bloco de instruções.

bool RuleBook::verificarEmpate(Board* tabuleiro) const { // Define o empate do exemplo genérico; esta implementação examina somente as coordenadas 0 a 2.
    if (tabuleiro == nullptr) { // Detecta a ausência de um tabuleiro antes de acessar seus métodos.
        throw std::invalid_argument("O ponteiro do tabuleiro não pode ser nulo."); // Lança uma exceção para informar que o chamador não forneceu um tabuleiro válido.
    } // Encerra este bloco de instruções.

    if (verificarVitoria(tabuleiro)) { // Se a verificação genérica encontrou vitória, o resultado não é empate.
        return false; // Retorna falso: a condição verificada por esta função não foi satisfeita.
    } // Encerra este bloco de instruções.

    // Se todas as posições estiverem preenchidas sem vencedor, configura empate
    for (int i = 0; i < 3; ++i) { // Percorre os índices 0, 1 e 2; esta regra genérica mantém o limite fixo do exemplo original.
        for (int j = 0; j < 3; ++j) { // Percorre as três primeiras colunas para cada linha examinada.
            if (tabuleiro->validarCoordenadas(i, j) && tabuleiro->consultarPosicao(i, j) == nullptr) { // Se a casa existir e estiver vazia, ainda falta preencher o setor examinado.
                return false; // Retorna falso: a condição verificada por esta função não foi satisfeita.
            } // Encerra este bloco de instruções.
        } // Encerra este bloco de instruções.
    } // Encerra este bloco de instruções.

    return true; // Retorna verdadeiro: a condição verificada por esta função foi satisfeita.
} // Encerra este bloco de instruções.
