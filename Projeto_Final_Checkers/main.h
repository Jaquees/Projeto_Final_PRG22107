#ifndef MAIN_H
#define MAIN_H

#include <QApplication>
#include <QGraphicsView>
#include <QGraphicsSceneDragDropEvent>

namespace CV {
// Vetor de estados do jogo
static const std::vector<std::string> gameStateVector = {
    "Movimento Inválido", "", "Vitória das Brancas", "Vitória das Pretas", "Empate"
    };
}


// Enumeração para os layouts do tabuleiro
typedef enum BoardLayout {
    Standard = 1
} BoardLayout;

// Classe que herda QGraphicsView
class GraphicsView : public QGraphicsView {
public:
    // Construtor da classe
    GraphicsView(QGraphicsScene *scene) : QGraphicsView(scene) {
        setAcceptDrops(true);
    }


private:
    // Manipuladores de eventos de arrastar e soltar
    void dragEnterEvent(QGraphicsSceneDragDropEvent *event) {
        Q_UNUSED(event);
    }

    void dragLeaveEvent(QGraphicsSceneDragDropEvent *event) {
        Q_UNUSED(event);
        update();
    }

    void dropEvent(QGraphicsSceneDragDropEvent *event) {
        event->setAccepted(true);
        update();
    }
};

// Funções para desenhar o tabuleiro e as peças
void drawBoard();
void drawPieces(std::map<std::pair<char, char>, char> &gameBoard);

// Função para redesenhar o tabuleiro após um movimento
void redrawBoard(std::pair<char, char> from, std::pair<char, char> to, QGraphicsScene *scene);

// Função principal do programa
int main(int argc, char *argv[]);

#endif
