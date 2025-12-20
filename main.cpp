#include "mainwindow.h"

#include <QApplication>
#include <QLabel>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Creating main window
    MainWindow w;
    w.setWindowTitle("PopChat");

    w.show();   // Ultimately hidden at the start
    return a.exec();
}
