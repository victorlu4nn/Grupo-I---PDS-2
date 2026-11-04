# Validação da entrega C9

Verificação realizada em 03/11/2026, no ambiente Linux disponível.

## Compilação e testes

- Compilação de todas as classes e dos testes em C++11 com g++.
- Verificação adicional com `-Wall -Wextra -Wpedantic -Werror`: sem avisos.
- Construção do aplicativo e da suite pelo `Makefile.c9` incluído: concluída.
- Doctest fornecido pelo usuário: **66 testes, 975 verificações, zero falhas**.
- AddressSanitizer e UndefinedBehaviorSanitizer: suite concluída sem relatos
  de acesso inválido à memória ou comportamento indefinido nos casos executados.
- **LeakSanitizer não pôde executar neste ambiente**, por restrição de ptrace.
  Os testes com ASan/UBSan foram executados com `ASAN_OPTIONS=detect_leaks=0`.
  Portanto, não se afirma comprovação dinâmica de ausência de vazamentos.

O teste verifica casos de sucesso e recusa de entradas; não constitui prova
formal de correção para toda entrada ou de todas as regras dos jogos.

## Cenários do aplicativo pelo terminal

Cada cenário foi executado como processo separado, com entrada controlada,
limite de tempo de 15 segundos e verificação do código de saída e da saída
esperada. Todos encerraram com código zero.

| Cenário | Resultado |
|---|---|
| EOF no menu, sem entrada | Passou |
| Opções com letras, sufixos, negativos e número enorme | Passou |
| Partida de Connect4 até vitória horizontal | Passou |
| Comando inválido em Damas seguido de jogada válida | Passou |
| Batalha Naval: regras, comando inválido, ataque e reinício | Passou |
| Linha de dez mil caracteres no menu | Passou |
| EOF durante confirmação de saída | Passou |
| Inicialização e encerramento dos três jogos em sequência | Passou |
| Partida de Connect4 até empate em 42 jogadas | Passou |

## Reproduzir a suite

Na pasta raiz do pacote:

```sh
make -f Makefile.c9 test
```

Para executar especificamente os novos cenários defensivos:

```sh
./testes_c9.exe --test-case="C9*"
```

Compilação opcional com sanitizadores em um ambiente g++ compatível:

```sh
make -f Makefile.c9 CXXFLAGS="-std=c++11 -Wall -Wextra -Wpedantic -g -fsanitize=address,undefined -fno-omit-frame-pointer" -B testes_c9.exe
./testes_c9.exe
```

Em uma máquina sem a restrição citada, mantenha a detecção de vazamentos
habilitada. A opção para desabilitá-la foi uma limitação do ambiente de
validação desta entrega.

O `compilar_c9.ps1` foi fornecido para Windows. PowerShell e MSVC não
estavam disponíveis para execução aqui. Os fontes e os novos caminhos foram
compilados com g++/Make no Linux. O pacote preserva o Makefile original do
repositório, oferecendo Makefile.c9 como alternativa separada.
