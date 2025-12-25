#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "chatbubble.h"
#include "settingswindow.h"
#include "theme.h"

#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QScrollArea>
#include <QScrollBar>
#include <QTimer>
#include <QGuiApplication>
#include <QScreen>
#include <QHotkey>


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
    this->setWindowTitle("PopChat");
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
    backgroundFrame->setStyleSheet(QString(
        "QFrame {"
        "   background-color: %1;"
        "   border-radius: 25px;"
        "   border: 1px solid #555;"
        "}"
    ).arg(Theme::Surface));
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
    scrollArea->setStyleSheet(QString(
        "QScrollArea { background: transparent; border: none; }"
        "QScrollBar:vertical { border: none; background: %1; width: 8px; margin: 0px; }"
        "QScrollBar::handle:vertical { background: #555; min-height: 20px; border-radius: 4px; }"
        "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical { height: 0px; }"
        "QWidget { background: transparent; }"
    ).arg(Theme::Surface));

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
    inputLayout->setSpacing(5);

    auto *inputBox = new QLineEdit(backgroundFrame);
    inputBox->setPlaceholderText("Ask...");
    inputBox->setStyleSheet(QString(
        "QLineEdit {"
        "   background-color: %1;"
        "   color: white;"
        "   border: 1px solid #555;"
        "   border-radius: 8px;"
        "   padding: 5px;"
        "   font-size: 14px;"
        "}"
        "QLineEdit:focus { border: 1px solid %2; }"
    ).arg(Theme::WindowBg).arg(Theme::Accent));
#ifdef Q_OS_MAC
    inputBox->setAttribute(Qt::WA_MacShowFocusRect, false);
#endif

    auto *sendButton = new QPushButton(backgroundFrame);
    sendButton->setCursor(Qt::PointingHandCursor);
    sendButton->setIcon(QIcon(":/icons/send.png"));
    sendButton->setIconSize(QSize(20, 20));
    sendButton->setStyleSheet(QString(
        "QPushButton {"
        "   background-color: %1;"
        "   border-radius: 8px;"
        "   padding: 5px 5px;"
        "}"
        "QPushButton:hover { background-color: %2; }"
        "QPushButton:pressed { background-color: %3; }"
    ).arg(Theme::Accent).arg(Theme::AccentHover).arg(Theme::AccentPressed));
    connect(inputBox, &QLineEdit::returnPressed, sendButton, &QPushButton::click);
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
        QTimer::singleShot(0, this, [this, scrollContent, scrollArea]() {
            this->fixPosition(scrollContent, scrollArea);
        });
    });

    auto settingsButton = new QPushButton(backgroundFrame);
    settingsButton->setCursor(Qt::PointingHandCursor);
    settingsButton->setIcon(QIcon(":/icons/setting.svg"));
    settingsButton->setIconSize(QSize(20, 20));
    settingsButton->setStyleSheet(QString(
        "QPushButton {"
        "   border-radius: 8px;"
        "   padding: 5px 5px;"
        "}"
        "QPushButton:hover { background-color: %1; }"
        "QPushButton:pressed { background-color: %2; }"
    ).arg(Theme::WindowBg).arg(Theme::Pressed));
    connect(settingsButton, &QPushButton::clicked, this, [this]() {
        if (!settingsWindow) {
            settingsWindow = new SettingsWindow(client, this);
        }
        settingsWindow->show();
		settingsWindow->raise();
		settingsWindow->activateWindow();
    });

    inputLayout->addWidget(inputBox);
    inputLayout->addWidget(settingsButton);
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

    const int minHeight = this->calculateHeight(scrollContent, scrollArea);
    resize(this->width(), minHeight + introLabel->height());

    this->setMinimumSize(250, minHeight);
    this->setMaximumSize(screenGeometry.width() / 1.5, screenGeometry.height() / 1.1);

    this->popWindow(inputBox);
}

MainWindow::~MainWindow()
{
    delete ui;
    ui = nullptr;
}

void MainWindow::fixPosition(QWidget *scrollContent, QScrollArea *scrollArea) {
    const int bottomAnchor = this->y() + this->height();

    this->setUpdatesEnabled(false);

    if (const int finalHeight = this->calculateHeight(scrollContent, scrollArea); this->height() <= finalHeight) {
        const int newY = bottomAnchor - finalHeight;
        this->setGeometry(this->x(), newY, this->width(), finalHeight);
    }
    this->setUpdatesEnabled(true);
    this->repaint();

    QTimer::singleShot(10, this, [scrollArea](){
        if (scrollArea->verticalScrollBar()->maximum() > 0) {
            scrollArea->verticalScrollBar()->setValue(scrollArea->verticalScrollBar()->maximum());
        }
    });
}

void MainWindow::popWindow(QLineEdit *inputBox) {
    this->show();
    this->raise();          // Put window on top
    this->activateWindow(); // Focus on window
    inputBox->setFocus();   // Ready to type
}

int MainWindow::calculateHeight(QWidget *scrollContent, const QScrollArea *scrollArea) const {
    scrollContent->adjustSize();

    const int contentHeight = scrollContent->layout()->sizeHint().height();
    const int overhead = this->height() - scrollArea->viewport()->height();
    const int targetHeight = contentHeight + overhead;

    constexpr int maxHeight = 550;
    const int finalHeight = std::min(targetHeight, maxHeight);

    return finalHeight;
}
