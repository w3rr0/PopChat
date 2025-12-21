#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QScrollArea>
#include <QScrollBar>
#include <QTimer>
#include <QGuiApplication>
#include <QScreen>

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
    this->resize(440, 92);

    // Screen
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->availableGeometry();

    int bottomMargin = std::min(screenGeometry.height() / 60, 70);
    int x = (screenGeometry.width() - this->width()) / 2;
    int y = screenGeometry.height() - this->height() - bottomMargin;

    this->move(x, y);

    // Frame
    auto *backgroundFrame = new QFrame(centralWidget);
    backgroundFrame->setStyleSheet(
        "QFrame {"
        "   background-color: #2D2D2D;"
        "   border-radius: 25px;"
        "   border: 1px solid #555;"
        "}"
    );
    mainLayout->addWidget(backgroundFrame);
    setCentralWidget(centralWidget);

    // Layout
    auto *contentLayout = new QVBoxLayout(backgroundFrame);
    contentLayout->setContentsMargins(15, 13, 15, 13);
    contentLayout->setSpacing(10);

    // Scroll Area
    auto *scrollArea = new QScrollArea(backgroundFrame);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);
    QSizePolicy scrollAreaPolicy = scrollArea->sizePolicy();
    scrollAreaPolicy.setVerticalPolicy(QSizePolicy::Ignored);
    scrollArea->setSizePolicy(scrollAreaPolicy);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea->setMinimumHeight(0);
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
#ifdef Q_OS_MAC
    inputBox->setAttribute(Qt::WA_MacShowFocusRect, false);
#endif

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

        if (introLabel && conversationLayout->indexOf(introLabel) != -1) {
            introLabel->hide();
            conversationLayout->removeWidget(introLabel);
            introLabel->deleteLater();
        }

        auto *msg = new ChatBubble(text, true, nullptr);
        conversationLayout->addWidget(msg);

        currentAnswerBubble = new ChatBubble("", false, nullptr);
        conversationLayout->addWidget(currentAnswerBubble);

        client->sendMessage(text);

        inputBox->clear();
        this->fixPosition(scrollContent, scrollArea);
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
            this->popWindow(inputBox);
        }
    });

    // Backend
    client = new OllamaClient(this);
    currentAnswerBubble = nullptr;

    connect(client, &OllamaClient::textReceived, this, [this, scrollArea, scrollContent](QString token) {
        if (currentAnswerBubble) {
            currentAnswerBubble->appendText(token);

            this->fixPosition(scrollContent, scrollArea);
        }
    });
    connect(client, &OllamaClient::replyFinished, this, [this]() {
        currentAnswerBubble = nullptr;
    });

    this->popWindow(inputBox);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::fixPosition(QWidget *scrollContent, QScrollArea *scrollArea) {
    int bottomAnchor = this->y() + this->height();
    scrollContent->adjustSize();
    //qApp->processEvents();

    int overhead = this->height() - scrollArea->viewport()->height();
    int contentHeight = scrollContent->sizeHint().height();
    int targetHeight = contentHeight + overhead;

    int maxHeight = 550;
    int finalHeight = std::min(targetHeight, maxHeight);

    int newY = bottomAnchor - finalHeight;
    this->setGeometry(this->x(), newY, this->width(), finalHeight);

    QTimer::singleShot(10, this, [scrollArea](){
        scrollArea->verticalScrollBar()->setValue(scrollArea->verticalScrollBar()->maximum());
    });
}

void MainWindow::popWindow(QLineEdit *inputBox) {
    this->show();
    this->raise();          // Put window on top
    this->activateWindow(); // Focus on window
    inputBox->setFocus();   // Ready to type
}
