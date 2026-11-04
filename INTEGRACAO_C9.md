# C9 no repositório principal

Este pacote está organizado conforme a estrutura mostrada no VS Code:
`include/base`, `include/games`, `src/base`, `src/games`, `src/main.cpp`,
`tests/base` e `tests/games`.

**Copie o conteúdo desta pasta para a raiz do repositório**, no mesmo nível
em que já estão `include`, `src`, `tests` e `Makefile`. Não coloque a pasta
`C9_Para_Repositorio` inteira dentro de `src`.

A integração foi preparada com os anexos desta conversa. O conteúdo do seu
Makefile, das configurações do VS Code e de alterações locais não enviadas não
foi recebido. Por isso, a compilação C9 usa um arquivo separado, `Makefile.c9`,
e também oferece um script para PowerShell.

## 1. Antes de copiar

No terminal do repositório, confira suas alterações e crie uma branch:

```powershell
git status
git switch -c melhoria-c9
```

Salve suas alterações atuais antes de substituir arquivos. Se o grupo alterou
algum arquivo desde a versão enviada nesta conversa, compare os dois conteúdos
antes de substituir esse arquivo. Não é necessário apagar nenhuma pasta do
repositório.

## 2. Onde colocar os arquivos

Os caminhos no ZIP já correspondem aos destinos. Copie as pastas, permitindo
mesclar diretórios e substituir somente os arquivos de mesmo nome que você
conferiu.

| Destino no repositório | Conteúdo do pacote | Ação |
|---|---|---|
| `include/base/` | Board, Game, GameManager, Move, Piece, Player e RuleBook (`.hpp`) | Atualizar junto com os `.cpp` correspondentes. |
| `include/base/Validacao.hpp` | Validação de texto e leitura limitada do terminal | Adicionar. |
| `include/games/` | Connect4, Damas e BatalhaNaval (`.hpp`) | Atualizar junto com os `.cpp` correspondentes. |
| `src/base/` | As sete classes base (`.cpp`) | Substituir as versões enviadas na conversa. |
| `src/games/` | Os três jogos (`.cpp`) | Substituir as versões enviadas na conversa. |
| `src/main.cpp` | Menu e tratamento de exceções | Substituir a main enviada, que imprimia apenas uma saudação. |
| `tests/base/` | Testes atualizados das classes base | Atualizar; foram retirados os `main` duplicados. |
| `tests/games/` | TesteConnect4.cpp, TesteDamas.cpp e TesteBatalhaNaval.cpp | Atualizar/adicionar conforme os nomes. |
| `tests/test_main.cpp` | Único main do doctest | Adicionar. |
| `tests/TesteDefensiva.cpp` | Testes negativos específicos do C9 | Adicionar. |
| `tests/ApoioTestes.hpp` | Peça auxiliar para testes de infraestrutura | Adicionar. |
| `include/doctest.h` | Framework enviado na conversa | Manter se já for o mesmo; o pacote inclui uma cópia. |
| `Makefile.c9` | Compilação com os caminhos existentes | Adicionar; não substitui seu `Makefile`. |
| `compilar_c9.ps1` | Compilação e execução de testes no Windows | Adicionar na raiz. |
| `design/C9.md` | Justificativa das alterações | Adicionar. |
| `design/VALIDACAO_C9.md` | Resultados e limites da validação | Adicionar. |

A lista exata está em `ARQUIVOS_C9.txt`. O pacote não contém substitutos para
`.vscode`, `visual`, seu README original, seu Makefile original ou os arquivos
de matrícula `.txt`.

**Esta é a versão integrada do pacote C9 anterior, adaptada aos caminhos do
seu repositório. Não é uma alteração restrita a main.cpp.** Algumas classes
precisaram de correções de compilação, validação e propriedade da memória.
Use cabeçalhos e implementações correspondentes, evitando misturar versões.

Se já houver `test_Connect4.cpp` ou outro arquivo antigo com os mesmos testes,
não o compile junto com `tests/games/TesteConnect4.cpp`. Os scripts fornecidos
usam uma lista explícita de fontes e não selecionam essas cópias antigas.

## 3. Como funciona o tratamento de exceções

A `main` trata falhas de inicialização, falta de memória e entrada/saída. O
laço de interação em `GameManager` trata erros de cada jogada e permite uma
nova tentativa. Os métodos das classes validam os argumentos antes de acessar
ponteiros, índices e casas do tabuleiro.

Por exemplo:

1. `Board` identifica uma coordenada inválida e lança `std::out_of_range`.
2. O código de interação captura a exceção por referência constante.
3. Uma mensagem é exibida, sem acessar a posição inválida.

Isso atende à ideia de tratar exceções na interface. O `catch` sozinho não
corrige um acesso indevido com `[]` nem um erro de compilação; por isso o
pacote inclui as validações nas classes.

## 4. Compilar no Windows / terminal do VS Code

Abra a raiz do repositório no terminal e confira:

```powershell
g++ --version
```

Se o comando ainda não for reconhecido, o compilador não está disponível no
PATH desse terminal. Configure o compilador e reabra o VS Code antes de
continuar; mudar o código C++ não corrige esse problema do ambiente.

Com g++ disponível, execute:

```powershell
.\compilar_c9.ps1
```

O script compila o aplicativo, compila a suite e executa os testes. Ele para
se qualquer etapa falhar. Depois, execute:

```powershell
.\jogos_c9.exe
```

Para repetir somente os testes:

```powershell
.\testes_c9.exe
```

Se o PowerShell impedir a execução do script por política local, utilize a
alternativa com Make abaixo, caso esteja instalada. Não é necessário alterar
as políticas do computador para compilar o projeto.

## 5. Alternativa com Make

No terminal que disponibilize GNU Make:

```sh
make -f Makefile.c9
make -f Makefile.c9 test
```

Se a ferramenta instalada se chamar `mingw32-make`, use esse nome:

```powershell
mingw32-make -f Makefile.c9
.\testes_c9.exe
.\jogos_c9.exe
```

Para aproveitar o Makefile antigo, ele precisa receber os diretórios de
inclusão abaixo e usar as listas de fontes indicadas em `Makefile.c9`:

```make
-Iinclude -Iinclude/base -Iinclude/games -Itests
```

O aplicativo usa `src/main.cpp`; a suite usa `tests/test_main.cpp`.
**Nunca inclua as duas mains no mesmo executável.** O doctest deve ter
`DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN` definido em somente um arquivo da suite.

## 6. Se o VS Code sublinhar os includes

A compilação fornecida já informa os diretórios corretos. Se apenas o editor
mostrar erro de include, confira o `includePath` da configuração ativa em
`.vscode/c_cpp_properties.json` e acrescente os caminhos abaixo, preservando
os demais campos e configurações existentes:

```json
"includePath": [
    "${workspaceFolder}/include",
    "${workspaceFolder}/include/base",
    "${workspaceFolder}/include/games",
    "${workspaceFolder}/tests"
]
```

Não substitua o arquivo inteiro por esse trecho: ele é somente uma propriedade
JSON. Use os comandos de compilação acima em vez de executar apenas o arquivo
`.cpp` aberto, pois o programa depende das outras classes do projeto.

## 7. Conferir o C9 no terminal

O menu oferece Connect4, Damas, Batalha Naval e saída. Experimente:

- Digitar `abc`, um número fora do menu e uma linha muito grande.
- No Connect4, tentar coluna `7` ou tentar jogar numa coluna cheia.
- Em Damas, digitar um comando incompleto como `a3 para`.
- Na Batalha Naval, digitar `A0`, `K5` ou repetir um ataque no mesmo turno do jogador.
- Usar `regras`, `reiniciar`, `sair partida` ou `sair programa`.
- Cancelar a saída com `n` e confirmar com `s`.

Os erros de entrada são informados sem mudar o turno. O fim da entrada (EOF)
encerra o programa sem laço infinito. A leitura é limitada a 256 caracteres.

Não é necessário arquivo externo de configuração, fonte, imagem ou cadastro
para inicializar os jogos. O doctest fornecido é usado somente nos testes.

## 8. Enviar para o repositório principal

Depois de copiar, compilar e testar:

```powershell
git status
git diff --stat
git diff --check
```

Revise as diferenças. Para adicionar somente os arquivos listados no pacote,
sem adicionar executáveis ou outras alterações do seu diretório:

```powershell
Get-Content .\ARQUIVOS_C9.txt | ForEach-Object { git add -- $_ }
git diff --cached --stat
git commit -m "Implementa validacoes e tratamento de excecoes do C9"
git push -u origin melhoria-c9
```

Depois abra um Pull Request de `melhoria-c9` para a branch principal usada pelo
grupo (`main` ou o nome adotado no projeto). Não é necessário usar `push --force`.
Esses comandos são instruções para você executar: o pacote não publicou nada
nem fez alterações no repositório remoto.

Para não listar os executáveis no Git, acrescente ao `.gitignore` existente:

```gitignore
/jogos_c9.exe
/testes_c9.exe
```

Se quiser registrar essa alteração no mesmo commit, faça também
`git add -- .gitignore` depois de revisar o conteúdo.

## Escopo e limites

O código é o mesmo conjunto C9 testado, com caminhos de compilação adaptados.
A validação do pacote adaptado usa g++/Make no Linux; o script PowerShell não
pôde ser executado neste ambiente. Consulte `design/VALIDACAO_C9.md`.

Damas tem captura obrigatória pela maior sequência, damas de movimento longo
e promoção ao fim do lance. O empate é simplificado: 40 meias-jogadas apenas
com damas, sem captura. Não inclui todos os critérios oficiais de torneio.
A alteração completa está descrita em `design/C9.md`.
