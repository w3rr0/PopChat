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

    // Styling
    w.setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool);
    w.setAttribute(Qt::WA_TranslucentBackground);
    w.setStyleSheet("background-color: #2D2D2D;  border-radius: 10px; border: 1px solid #555;");

    // Hotkey
    QHotkey *hotkey = new QHotkey(QKeySequence("Ctrl+Shift+K"), true, &a);

    QObject::connect(hotkey, &QHotkey::activated, [&w]() {
        if (w.isVisible()) {
            w.hide();
        } else {
            w.show();
            w.activateWindow(); // Focus on window
            w.raise();
        }
    });

    // Content
    QVBoxLayout *layout = new QVBoxLayout(&w);
    QLabel *label = new QLabel("Press Ctrl+Shift+K", &w);
    label->setStyleSheet("color: white; font-size: 18px; border: none;");
    label->setAlignment(Qt::AlignCenter);
    layout->addWidget(label);

    w.show();
    return a.exec();
}
