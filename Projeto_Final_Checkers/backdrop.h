#ifndef BACKDROP_H
#define BACKDROP_H

#include <QGraphicsItem>
#include <QGraphicsSceneDragDropEvent>
#include <QMimeData>

class Backdrop : public QGraphicsItem
{
public:
    // Construtor da classe Backdrop
    Backdrop()
    {
        setAcceptDrops(true);
    }

private:
    QColor colour = Qt::white;

    // Manipula o evento de arrastar sobre o item
    void dragEnterEvent(QGraphicsSceneDragDropEvent *event)
    {
        if (event->mimeData()->hasText())
        {
            event->setAccepted(true);
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
        update();
    }

    // Manipula o evento de soltar o item após o arraste
    void dropEvent(QGraphicsSceneDragDropEvent *event)
    {
        event->setAccepted(true);

        // Verifica se os dados do MIME contêm texto
        if (event->mimeData()->hasText())
        {
            // Obtém a string dos dados do MIME
            std::string s = event->mimeData()->text().toStdString();

            // Atualiza o tabuleiro com o movimento do usuário, usando uma posição de destino inválida
            redrawBoard(std::make_pair(char(s.at(0)), char(s.at(1))), std::make_pair('z', 'z'), scene());
        }
        else
        {
            update();
        }
    }

    // Retorna o retângulo delimitador do item
    QRectF boundingRect() const
    {
        return QRectF(75, 85, 600, 600);
    }

    // Pinta o item
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0)
    {
        Q_UNUSED(option);
        Q_UNUSED(widget);

        // Define a cor de preenchimento
        painter->setBrush(colour);
        painter->drawRect(boundingRect());
    }
};

#endif // BACKDROP_H
