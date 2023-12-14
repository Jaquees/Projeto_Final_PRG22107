#include <QTimer>
#include <QPushButton>
#include <QComboBox>
#include <QMessageBox>
#include <QCheckBox>
#include "main.h"
#include "checkersgame.h"
#include "backdrop.h"
#include "boardsquare.h"
#include "gamepiece.h"
#include <map>


// Declaração dos namespaces para evitar conflitos de nome
namespace CV {
// Mapa representando o tabuleiro do jogo
std::map<std::pair<char, char>, char> gameBoard;

// Variáveis de controle do jogo
int playerTurn = White; // Define de quem é a vez (Branco ou Vermelho)
int boardLayout = Standard; // Define o layout do tabuleiro (Padrão ou customizado)
int gameStatus = ValidMove; // Estado do jogo (Movimento válido, vitória Branca, vitória Preta, Empate, etc.)
}

namespace CF {
// Flags de controle do jogo
bool resetFlag = false; // Sinaliza reinício do jogo
bool playerMovingFlag = false; // Evita interrupções durante o movimento do jogador
}

// Função para desenhar o tabuleiro na cena
void drawSceneBoard(QGraphicsScene& scene) {
    int yOffset = 85;

    // Adiciona o título "DAMAS" à cena
    QGraphicsTextItem* titleText = scene.addText(QString("DAMAS"));
    titleText->setFont(QFont("Georgia", 55));
    titleText->setPos(240, 10);
    titleText->setDefaultTextColor(QColor(255,255,255));

    // Exibe de quem é a vez (Branco ou Preto) na cena
    QGraphicsTextItem* playerTurnText = scene.addText((CV::playerTurn == White) ? QString("Vez das brancas") : QString("Vez das vermelhas"));
    playerTurnText->setFont(QFont("Calibri", 16));
    playerTurnText->setPos(690, 130);
    playerTurnText->setDefaultTextColor(QColor(255,255,255));

    // Exibe o estado do jogo (movimento válido, vitória, empate, etc.) na cena
    QGraphicsTextItem* displayBar = scene.addText(QString(CV::gameStateVector.at(CV::gameStatus).c_str()));
    displayBar->setFont(QFont("Calibri", 22));
    displayBar->setPos(620 + 75, 30);

    // Cria um botão de reset na cena
    QPushButton* resetButton = new QPushButton;
    QObject::connect(resetButton, &QPushButton::clicked, []() { CF::resetFlag = true; });
    resetButton->setFont(QFont("Calibri", 14));
    resetButton->setGeometry(QRect(695, 100, 120, 30));
    resetButton->setText("Resetar Jogo");
    scene.addWidget(resetButton);

    // Adiciona um item de plano de fundo à cena
    QGraphicsItem* BackdropItem = new Backdrop();
    scene.addItem(BackdropItem);

    // Cria os quadrados pretos do tabuleiro
    for (char y = '1'; y <= '8'; y++) {
        for (char x = 'a'; x <= 'h'; x++) {
            if ((((y - 49) % 2) == 0) == (((x - 97) % 2) == 0)) {
                // Cria um quadrado do tabuleiro e adiciona à cena
                QGraphicsItem* boardSquareItem = new BoardSquare((x - 97) * 75 + 75, 525 - (y - 49) * 75 + yOffset, std::make_pair(x, y));
                scene.addItem(boardSquareItem);
            }
        }
    }
}

// Função para desenhar as peças na cena
void drawScenePieces(QGraphicsScene& scene, std::map<std::pair<char, char>, char>& gameBoard) {
    int yOffset = 85;
    QColor colour = Qt::white;

    // Itera sobre o tabuleiro
    for (char y = '1'; y <= '8'; y++) {
        for (char x = 'a'; x <= 'h'; x++) {
            if ((((y - 49) % 2) == 0) == (((x - 97) % 2) == 0)) {
                char piece = gameBoard[std::make_pair(x, y)];
                bool king = false;

                // Determina a cor e se a peça é uma dama
                if (piece == pieces[Black]) {
                    colour = QColor(170, 0, 0);
                } else if (piece == pieces[BlackKing]) {
                    colour = QColor(170, 0, 0);
                    king = true;
                } else if (piece == pieces[White]) {
                    colour = Qt::white;
                } else if (piece == pieces[WhiteKing]) {
                    colour = Qt::white;
                    king = true;
                } else {
                    continue; // Se não há peça, passa para a próxima iteração
                }

                // Cria uma peça e a adiciona à cena
                QGraphicsItem* gamePieceItem = new GamePiece((x - 97) * 75 + 75, 525 - (y - 49) * 75 + yOffset, colour, std::make_pair(x, y), king);
                scene.addItem(gamePieceItem);
            }
        }
    }
}

// Função para redesenhar o tabuleiro após um movimento
void redrawBoard(std::pair<char, char> from, std::pair<char, char> to, QGraphicsScene* scene) {
    CF::playerMovingFlag = true;
    scene->clear();

    if (CV::gameStatus != WhiteWin && CV::gameStatus != BlackWin && CV::gameStatus != Draw) {
        takeTurn(CV::gameBoard, std::make_pair(from, to), CV::playerTurn);
    }

    // Redesenha o tabuleiro e as peças na cena
    drawSceneBoard(*scene);
    drawScenePieces(*scene, CV::gameBoard);
    CF::playerMovingFlag = false;
}


int main(int argc, char* argv[]) {
    QApplication a(argc, argv);

    int width = 700;
    int height = 700;
    QGraphicsScene scene(0, 0, width, height);

    // Inicializa o tabuleiro e desenha o tabuleiro e as peças na cena
    resetBoard(CV::gameBoard);
    drawSceneBoard(scene);
    drawScenePieces(scene, CV::gameBoard);

    GraphicsView view(&scene);
    QRectF bounds = scene.itemsBoundingRect();
    bounds.setWidth(bounds.width() * 0.9);
    bounds.setHeight(bounds.height() * 0.9);

    view.setBackgroundBrush(QColor(11, 93, 88));
    view.setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    view.setWindowTitle("Projeto Final de Orientada a Objetos");
    view.showMaximized();

    QTimer* timer = new QTimer;
    QObject::connect(timer, &QTimer::timeout, [&scene]() {
        if (CF::resetFlag) {

            resetBoard(CV::gameBoard);

            checkPromote(CV::gameBoard);
            CV::playerTurn = White;
            scene.clear();
            drawSceneBoard(scene);
            drawScenePieces(scene, CV::gameBoard);
            CF::resetFlag = false;
        }
    });

    timer->start(100);
    view.show();

    return a.exec();
}
