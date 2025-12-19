#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QFrame>
#include <QLineEdit>
#include <QPushButton>
#include <QScrollArea>
#include <QScrollBar>
#include <QTimer>

#include "chatbubble.h"

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
    this->resize(450, 100);

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

    // Scroll Area
    auto *scrollArea = new QScrollArea(backgroundFrame);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea->setStyleSheet(
        "QScrollArea { background: transparent; border: none; }"
        "QScrollBar:vertical { border: none; background: #2D2D2D; width: 8px; margin: 0px; }"
        "QScrollBar::handle:vertical { background: #555; min-height: 20px; border-radius: 4px; }"
        "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical { height: 0px; }"
        "QWidget { background: transparent; }"
    );

    auto *scrollContent = new QWidget();

    auto *conversationLayout = new QVBoxLayout(scrollContent);
    conversationLayout->setContentsMargins(0, 0, 0, 0);
    conversationLayout->setSpacing(10);
    conversationLayout->setAlignment(Qt::AlignTop);

    scrollArea->setWidget(scrollContent);
    contentLayout->addWidget(scrollArea, 1);

    auto *introLabel = new QLabel("Press Ctrl+Shift+K", backgroundFrame);
    introLabel->setStyleSheet("color: #AAAAAA; font-size: 14px; border: none; background: transparent;");
    introLabel->setAlignment(Qt::AlignCenter);
    conversationLayout->addWidget(introLabel);

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
    connect(inputBox, &QLineEdit::returnPressed, sendButton, &QPushButton::animateClick);
    connect(sendButton, &QPushButton::clicked, this, [this, inputBox, conversationLayout, introLabel, scrollArea, scrollContent]() {
        QString text = inputBox->text().trimmed();
        if (text.isEmpty()) return;
        if (conversationLayout->indexOf(introLabel) != -1) {
            introLabel->deleteLater();
        }

        auto *msg = new ChatBubble(text, true, nullptr);
        conversationLayout->addWidget(msg);

        auto *answer = new ChatBubble("Answer for you", false, nullptr);
        conversationLayout->addWidget(answer);

        inputBox->clear();

        scrollContent->adjustSize();
        qApp->processEvents();

        int requiredHeight = scrollContent->sizeHint().height() + 100;
        int maxHeight = 550;

        if (requiredHeight < maxHeight) {
            this->resize(this->width(), requiredHeight);
        } else {
            this->resize(this->width(), maxHeight);
        }

        QTimer::singleShot(10, this, [scrollArea](){
            scrollArea->verticalScrollBar()->setValue(scrollArea->verticalScrollBar()->maximum());
        });

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
