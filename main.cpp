#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Creating main window
    MainWindow w;
    w.setWindowTitle("PopChat");
    w.resize(400, 200);

    // Styling
    w.setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool);
    w.setAttribute(Qt::WA_TranslucentBackground);
    w.setStyleSheet("background-color: #2D2D2D;  border-radius: 10px; border: 1px solid #555;");

    w.show();
    return a.exec();
}
