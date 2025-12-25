#include "settingswindow.h"
#include <QtWidgets/qboxlayout.h>
#include "ollamaclient.h"
#include "theme.h"

#include <QPushButton>
#include <QSettings>
#include <QLineEdit>
#include <QGuiApplication>
#include <QLabel>

SettingsWindow::SettingsWindow(QWidget *parent)
    : QDialog(parent)
{
    this->setWindowTitle("Settings");

    // Window
    auto* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    // Frame
    auto* backgroundFrame = new QFrame(this);
    backgroundFrame->setStyleSheet(QString(
        "QFrame {"
        "   background-color: %1;"
        "}"
    ).arg(Theme::Surface));
    mainLayout->addWidget(backgroundFrame);

    auto *bgLayout = new QVBoxLayout(backgroundFrame);

    auto *innerFrame = new QFrame(backgroundFrame);
    innerFrame->setStyleSheet(QString(
        "QFrame {"
        "   background-color: %1;"
        "   border-radius: 15px;"
        "   border-color: black;"
        "   padding: 10px"
        "}"
    ).arg(Theme::WindowBg));
    innerFrame->setMaximumSize(500, 500);
    bgLayout->addWidget(innerFrame, 0, Qt::AlignCenter);

    // Layout
    auto* contentLayout = new QVBoxLayout(innerFrame);
    contentLayout->setContentsMargins(15, 15, 15, 15);
    contentLayout->setSpacing(10);

    auto *settingFrame = new QFrame(innerFrame);
    settingFrame->setContentsMargins(0, 0, 0, 0);
    auto *settingLayout = new QHBoxLayout(settingFrame);
    settingLayout->setContentsMargins(0, 0, 0, 0);
    settingLayout->setSpacing(10);
    settingLayout->setSizeConstraint(QLayout::SetFixedSize);

    auto *label = new QLabel("Model Name:", settingFrame);
    label->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
    label->setStyleSheet(
        "padding: 0px;"
    );
    settingLayout->addWidget(label, 0);

    auto *modelInput = new QLineEdit(settingFrame);
    modelInput->setPlaceholderText("ex. llama3");
    modelInput->setStyleSheet(QString(
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
    settingLayout->addWidget(modelInput, 1);

    contentLayout->addWidget(settingFrame);

    auto *saveButton = new QPushButton("Save", this);
    saveButton->setStyleSheet(QString(
        "QPushButton {"
        "   background-color: %1;"
        "   border-radius: 8px;"
        "   padding: 5px 5px;"
        "   color: %4;"
        "}"
        "QPushButton:hover { background-color: %2; }"
        "QPushButton:pressed { background-color: %3; }"
    ).arg(Theme::Accent).arg(Theme::AccentHover).arg(Theme::AccentPressed).arg(Theme::TextMain));
    contentLayout->addWidget(saveButton);

    connect(saveButton, &QPushButton::clicked, this, [this, modelInput]() {
        this->saveSettings(modelInput);
        this->hide();
        });

    this->setMaximumSize(innerFrame->sizeHint().width() + 100, innerFrame->sizeHint().height() + 100);
    this->resize(this->maximumSize());
}

void SettingsWindow::saveSettings(QLineEdit *modelInput) {
    QSettings settings("model");

    const QString newModel = modelInput->text().trimmed();
    if (newModel.isEmpty()) {
        return;
    }

    OllamaClient::setModelName(newModel);
    settings.sync();
}
