#include <QApplication>
#include "mainwindow.h"


int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    app.setFont(QFont("Verdana", 9));

    MainWindow mainWindow;
    mainWindow.show();

    return app.exec();
}
