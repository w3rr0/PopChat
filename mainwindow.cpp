#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QFrame>
#include <QLineEdit>
#include <QPushButton>

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
    auto *centralWidget = new QWidget(this);
    auto *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    this->statusBar()->hide();

    // Frame
    auto *backgroundFrame = new QFrame(centralWidget);
    backgroundFrame->setStyleSheet(
        "QFrame {"
        "   background-color: #2D2D2D;"
        "   border-radius: 15px;"
        "   border: 1px solid #555;"
        "}"
    );
    mainLayout->addWidget(backgroundFrame);
    setCentralWidget(centralWidget);

    // Layout
    auto *contentLayout = new QVBoxLayout(backgroundFrame);
    contentLayout->setContentsMargins(20, 20, 20, 20);
    contentLayout->setSpacing(10);

    auto *conversationLayout = new QVBoxLayout();
    auto *introLabel = new QLabel("Wciśnij Ctrl+Shift+K", backgroundFrame);
    introLabel->setStyleSheet("color: #AAAAAA; font-size: 14px; border: none; background: transparent;");
    introLabel->setAlignment(Qt::AlignCenter);
    conversationLayout->addWidget(introLabel);
    contentLayout->addLayout(conversationLayout, 1);

    auto *inputLayout = new QHBoxLayout();
    inputLayout->setSpacing(10);

    auto *inputBox = new QLineEdit(backgroundFrame);
    inputBox->setPlaceholderText("Ask...");
    inputBox->setStyleSheet(
        "QLineEdit {"
        "   background-color: #404040;"
        "   color: white;"
        "   border: 1px solid #555;"
        "   border-radius: 8px;"
        "   padding: 5px;"
        "   font-size: 14px;"
        "}"
        "QLineEdit:focus { border: 1px solid #0078D7; }"
    );

    auto *sendButton = new QPushButton("Send", backgroundFrame);
    sendButton->setCursor(Qt::PointingHandCursor);
    sendButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #0078D7;"
        "   color: white;"
        "   border-radius: 8px;"
        "   padding: 5px 15px;"
        "   font-weight: bold;"
        "}"
        "QPushButton:hover { background-color: #0063B1; }"
        "QPushButton:pressed { background-color: #004C87; }"
    );
    connect(sendButton, &QPushButton::clicked, this, [this, inputBox, conversationLayout, introLabel]() {
        if (conversationLayout->indexOf(introLabel) != -1) {
            introLabel->deleteLater();
        }
        if (!inputBox->text().isEmpty()) {
            auto *label = new QLabel(inputBox->text());
            conversationLayout->addWidget(label, 0);
            inputBox->clear();
        }
    });

    inputLayout->addWidget(inputBox);
    inputLayout->addWidget(sendButton);

    contentLayout->addLayout(inputLayout, 0);   // Takes as little space as needed

    // Hotkey
    hotkey = new QHotkey(QKeySequence("Ctrl+Shift+K"), true, this);
    connect(hotkey, &QHotkey::activated, this, [this, inputBox]() {
        if (this->isVisible()) {
            this->hide();
        } else {
            this->show();
            this->raise();          // Put window on top
            this->activateWindow(); // Focus on window
            inputBox->setFocus();
        }
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}
