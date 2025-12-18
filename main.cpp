#include "mainwindow.h"

#include <QApplication>
#include <QHotkey>
#include <QVBoxLayout>
#include <QLabel>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Creating main window
    MainWindow w;
    w.setWindowTitle("PopChat");
    w.resize(400, 200);

    w.show();   // Ultimately hidden at the start
    return a.exec();
}
