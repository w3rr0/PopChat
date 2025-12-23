#include "settingswindow.h"
#include "ollamaclient.h"

#include <QPushButton>
#include <QSettings>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QGuiApplication>

SettingsWindow::SettingsWindow(QWidget *parent)
    : QDialog(parent)
{
    this->setWindowTitle("Settings");

    // Window
    auto* centralWidget = new QWidget(this);
    auto* mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    // Frame
    auto* backgroundFrame = new QFrame(centralWidget);
    backgroundFrame->setStyleSheet(
        "QFrame {"
        "   background-color: #2D2D2D;"
        "   border-radius: 25px;"
        "   border: 1px solid #555;"
        "}"
    );
    mainLayout->addWidget(backgroundFrame);

    // Layout
    auto* contentLayout = new QVBoxLayout(backgroundFrame);
    contentLayout->setContentsMargins(15, 13, 15, 13);
    contentLayout->setSpacing(10);

    auto *modelInput = new QLineEdit(this);
    contentLayout->addWidget(modelInput);

    auto *saveButton = new QPushButton("Save", this);
    contentLayout->addWidget(saveButton);

    connect(saveButton, &QPushButton::clicked, this, [this, modelInput]() {
        this->saveSettings(modelInput);
        this->hide();
        });
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
