#include <QApplication>
#include <QPushButton>
#include <QLabel>
#include <QGridLayout>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QWidget *mainWindow = new QWidget();
    mainWindow->setWindowTitle("Xadrezinho dicria");
    mainWindow->showFullScreen();

    // Labels
    QLabel *label1 = new QLabel("Bem vindo");
    label1->setAlignment(Qt::AlignCenter);

    // Buttons
    QPushButton *bPlay = new QPushButton("New Game");
    QPushButton *bSet = new QPushButton("Settings");
    QPushButton *bQuit = new QPushButton("Quit");

    // Layout
    QGridLayout *mainLayout = new QGridLayout();

    // Adding widgets to the layout
    mainLayout->addWidget(label1, 0, 0, 1, 3);
    mainLayout->addWidget(bPlay, 1, 0);
    mainLayout->addWidget(bSet, 1, 1);
    mainLayout->addWidget(bQuit, 1, 2);

    // Set the layout on the main window
    mainWindow->setLayout(mainLayout);

    // Connect button signals to slots
    QObject::connect(bPlay, SIGNAL(clicked()), &a, SLOT(quit())); // You need to connect this to a slot.
    QObject::connect(bSet, SIGNAL(clicked()), &a, SLOT(quit()));  // You need to define the slot to open settings.
    QObject::connect(bQuit, SIGNAL(clicked()), &a, SLOT(quit()));

    // Show the main window
    mainWindow->show();

    return a.exec();
}
