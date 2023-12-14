#ifndef BOARDSQUARE_H
#define BOARDSQUARE_H

#include <QGraphicsItem>
#include <QGraphicsSceneDragDropEvent>
#include <QMimeData>
#include "main.h"

class BoardSquare : public QGraphicsItem
{
private:
    int x = 0;
    int y = 0;
    std::pair<char, char> square;
    QColor colour = Qt::black;
    bool dragOver = false;

public:
    // Construtor da classe BoardSquare
    BoardSquare(int x, int y, std::pair<char, char> square)
    {
        this->x = x;
        this->y = y;
        this->square = square;
        setAcceptDrops(true);
    }

    // Define a posição da instância
    void setPos(int x, int y)
    {
        this->x = x;
        this->y = y;
    }

    // Obtém as coordenadas x e y da instância
    std::pair<int, int> get()
    {
        return std::make_pair(x, y);
    }

private:
    // Manipula o evento de arrastar sobre o item
    void dragEnterEvent(QGraphicsSceneDragDropEvent *event)
    {
        if (event->mimeData()->hasText())
        {
            event->setAccepted(true);
            dragOver = true;
            update();
        }
        else
        {
            event->setAccepted(false);
        }
    }

    // Manipula o evento de sair do arraste sobre o item
    void dragLeaveEvent(QGraphicsSceneDragDropEvent *event)
    {
        Q_UNUSED(event);
        dragOver = false;
        update();
    }

    // Manipula o evento de soltar o item após o arraste
    void dropEvent(QGraphicsSceneDragDropEvent *event)
    {
        dragOver = false;
        event->setAccepted(true);

        // Verifica se os dados do MIME contêm texto
        if (event->mimeData()->hasText())
        {
            // Obtém a string dos dados do MIME
            std::string s = event->mimeData()->text().toStdString();

            // Atualiza o tabuleiro com o movimento do usuário
            redrawBoard(std::make_pair(char(s.at(0)), char(s.at(1))), square, scene());
        }
        else
        {
            update();
        }
    }

    // Retorna o retângulo delimitador do item
    QRectF boundingRect() const
    {
        return QRectF(x, y, 75, 75);
    }

    // Pinta o item
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0)
    {
        Q_UNUSED(option);
        Q_UNUSED(widget);

        // Define a cor de preenchimento com base no estado de arraste
        painter->setBrush(dragOver ? Qt::lightGray : colour);
        painter->drawRect(boundingRect());
    }
};

#endif // BOARDSQUARE_H
