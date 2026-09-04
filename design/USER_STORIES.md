# User Stories

Nesse arquivo, iremos escrever os user stories. Atualmente, temos dois usuários para o qual nosso projeto será voltado: o jogador e o desenvolvedor. Evidenciamos quais seriam as funcionalidades e objetivos de interesse para cada usuário, sendo uma breve descrição do que seria e condições de aceitação que irão validar que tal objetivo seja atingido.

## Jogador

### User Story 1
#### Descrição: 
Como jogador, quero ver o catálogo de jogos de tabuleiro disponíveis e selecionar um deles na engine para poder iniciar uma partida do jogo desejado.
#### Critérios de aceitação: 
• O sistema deve exibir os jogos disponíveis;
• O jogador deve poder selecionar um dos jogos exibidos;
• O sistema deve carregar as configurações correspondentes ao jogo selecionado;
• O programa deve impedir a seleção de um jogo não disponível, avisar o jogador e esperar uma seleção válida.

### User Story 2
#### Descrição: 
Como jogador, quero consultar as regras do jogo selecionado para entender como jogar corretamente.
#### Critérios de aceitação: 
• O jogador deve poder acessar as regras antes de iniciar uma partida;
• O jogador deve poder consultar as regras durante uma partida;
• As regras exibidas devem corresponder ao jogo selecionado;
• O sistema deve apresentar as condições de vitória do jogo;
• O sistema deve redesenhar o estado atual do tabuleiro após a saída da tela de regras.

### User Story 3
#### Descrição: 
Como jogador, quero iniciar uma nova partida para poder jogar o jogo selecionado.
#### Critérios de aceitação: 
• O sistema deve criar uma nova partida com o estado inicial correto;
• O tabuleiro deve ser configurado de acordo com o jogo selecionado;
• O sistema deve definir quem realiza a primeira jogada, de acordo com as regras do jogo;
• O sistema deve exibir o estado inicial da partida.

### User Story 4
#### Descrição: 
Como jogador, quero realizar jogadas durante minha vez para avançar na partida.
#### Critérios de aceitação: 
• O sistema deve permitir jogadas apenas ao jogador da vez;
• O sistema deve validar a jogada de acordo com as regras do jogo;
• O sistema deve impedir jogadas inválidas;
• O sistema deve atualizar o estado do tabuleiro após uma jogada válida;
• O sistema deve informar ao jogador quando uma jogada for inválida.

### User Story 5
#### Descrição: 
Como jogador, quero sair ou reiniciar uma partida para não precisar continuar uma partida que não desejo finalizar.
#### Critérios de aceitação: 
• O jogador deve poder encerrar uma partida antes do seu término;
• O sistema deve solicitar confirmação antes de encerrar a partida;
• O jogador deve poder iniciar uma nova partida após encerrar a anterior;
• Ao reiniciar, o sistema deve restaurar o estado inicial do jogo;
• O sistema não deve considerar uma partida abandonada como vitória de um jogador, exceto quando as regras do jogo definirem o contrário.

### User Story 6
#### Descrição: 
Como jogador, quero que o sistema identifique automaticamente o fim da partida para saber quando o jogo terminou e quem venceu.
#### Critérios de aceitação: 
• O sistema deve verificar as condições de fim após cada jogada;
• O sistema deve identificar quando um jogador atingir uma condição de vitória;
• O sistema deve identificar empates quando aplicável às regras do jogo;
• O sistema deve informar claramente o resultado da partida;
• Após o término, o sistema deve impedir novas jogadas na partida finalizada.

## Desenvolvedor

### User Story 7
#### Descrição: 
Como programador, desejo instanciar um tabuleiro definindo dinamicamente suas dimensões, para suportar jogos além do grid tradicional 8x8.
#### Critérios de aceitação: 
• A classe base Board deve permitir inicialização de matrizes N x M via construtor;
• O sistema deve gerenciar a memória do grid dinamicamente e de forma segura;
• O sistema deve lançar exceções caso dimensões negativas ou zeradas sejam solicitadas.

### User Story 8
#### Descrição: 
Como programador, desejo registrar meu novo jogo no menu principal da engine utilizando uma interface padronizada, sem precisar alterar a lógica de renderização do menu.
#### Critérios de aceitação: 
• A engine deve possuir um gerenciador de instâncias;
• O menu deve iterar sobre a lista de jogos registrados no código para gerar a interface de seleção automaticamente;
• O sistema deve associar corretamente a escolha numérica do usuário no console à instância da classe correspondente.