#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QVBoxLayout>
#include <QLabel>
#include <QFrame>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Ui
    setWindowFlags(
        Qt::FramelessWindowHint |
        Qt::WindowStaysOnTopHint |
        Qt::Tool
    );
    setAttribute(Qt::WA_TranslucentBackground);

    // Window
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    // Frame
    QFrame *backgroundFrame = new QFrame(centralWidget);
    backgroundFrame->setStyleSheet(
        "QFrame {"
        "   background-color: #2D2D2D;"
        "   border-radius: 15px;"
        "   border: 1px solid #555;"
        "}"
    );
    mainLayout->addWidget(backgroundFrame);
    setCentralWidget(centralWidget);

    // Label
    QVBoxLayout *contentLayout = new QVBoxLayout(backgroundFrame);
    QLabel *label = new QLabel("Wciśnij Ctrl+Shift+K", backgroundFrame);
    label->setStyleSheet("color: white; font-size: 18px; border: none; background: transparent;");
    label->setAlignment(Qt::AlignCenter);
    contentLayout->addWidget(label);

    // Hotkey
    hotkey = new QHotkey(QKeySequence("Ctrl+Shift+K"), true, this);
    connect(hotkey, &QHotkey::activated, this, [this]() {
        if (this->isVisible()) {
            this->hide();
        } else {
            this->show();
            this->raise();          // Put window on top
            this->activateWindow(); // Focus on window
        }
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}
