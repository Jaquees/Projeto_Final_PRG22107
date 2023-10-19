#include "mainwindow.h"

#include <QApplication>
#include <QPushButton>
#include <QLabel>
#include <QLayout>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QWidget *mainWindow = new QWidget();
    mainWindow->setWindowTitle("Xadrezinho dicria");
    mainWindow->showFullScreen();

    // labels
    QLabel *label1 = new QLabel("Bem vindo");
    label1->setAlignment(Qt::AlignCenter);

    // Buttons
    QPushButton *bPlay = new QPushButton("New Game");
    QObject::connect(bPlay, SIGNAL(clicked()), &a, SLOT( ));
    QPushButton *bSet = new QPushButton("Settings");
    QObject::connect(bSet, SIGNAL(clicked()), &a, SLOT(open() ));
    QPushButton *bQuit = new QPushButton("Quit");
    QObject::connect(bQuit, SIGNAL(clicked()), &a, SLOT(quit()));
    QGridLayout *mainLayout = new QGridLayout();

    // Adding to main layout
    mainLayout->addWidget(label1);
    mainLayout->addWidget(bPlay);
    mainLayout->addWidget(bSet);
    mainLayout->addWidget(bQuit);

    // seting layout on main window
    mainWindow->setLayout(mainLayout);

    // showing main window
    mainWindow->show();
    /*MainWindow w;
    w.show();*/
    return a.exec();
}
