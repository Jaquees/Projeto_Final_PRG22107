# Projeto_Final_PRG22107
Damas
Inicialização do Jogo:

    Estrutura do Tabuleiro: O tabuleiro é representado por uma matriz bidimensional (gameBoard). Cada posição do tabuleiro é identificada por um par de coordenadas (coluna, linha). A inicialização do tabuleiro ocorre na função resetBoard.

    Interface Gráfica: A interface gráfica é gerenciada pela biblioteca Qt. A cena (QGraphicsScene) exibe o tabuleiro e as peças, enquanto a classe GraphicsView é responsável por renderizar a cena.

Lógica do Jogo:

    Movimentos e Estado do Jogo: O estado do jogo é controlado pelas variáveis, playerTurn indica de quem é a vez de jogar (branco ou vermelho). boardLayout define o layout do tabuleiro, e gameStatus indica o estado do jogo (por exemplo, se há um movimento válido, vitória branca, vitória preta, empate).

    Desenho do Tabuleiro e Peças: A função drawSceneBoard desenha o tabuleiro na cena, exibindo o título "DAMAS", informando de quem é a vez e exibindo o estado do jogo. A função drawScenePieces desenha as peças no tabuleiro.

    Atualização da Interface: A função redrawBoard é responsável por redesenhar o tabuleiro e as peças após cada movimento. Isso envolve limpar a cena, realizar o movimento no tabuleiro e, em seguida, redesenhar o tabuleiro e as peças atualizados.

Interação do Usuário:

    Botão de Reset: Um botão "Resetar Jogo". Quando pressionado, a variável resetFlag é definida como verdadeira, e o tabuleiro é reiniciado.

Temporizador:

Um temporizador (QTimer) é usado para verificar periodicamente se o jogo precisa ser reiniciado. Isso é feito verificando a variável resetFlag. Se ela estiver definida, o jogo é reiniciado e a flag é resetada.


Diagramas: 
[Projeto_casos_uso.drawio.pdf](https://github.com/Jaquees/Projeto_Final_PRG22107/files/13672454/Projeto_casos_uso.drawio.pdf)
[Diagrama_Classes.drawio.pdf](https://github.com/Jaquees/Projeto_Final_PRG22107/files/13672455/Diagrama_Classes.drawio.pdf)
