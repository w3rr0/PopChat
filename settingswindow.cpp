#include "settingswindow.h"
#include <QtWidgets/qboxlayout.h>
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
    auto* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    // Frame
    auto* backgroundFrame = new QFrame(this);
    backgroundFrame->setStyleSheet(
        "QFrame {"
        "   background-color: #202020;"
        "}"
    );
    mainLayout->addWidget(backgroundFrame);

    auto *bgLayout = new QVBoxLayout(backgroundFrame);

    auto *innerFrame = new QFrame(backgroundFrame);
    innerFrame->setStyleSheet(
        "QFrame {"
        "   background-color: #2D2D2D;"
        "   border-radius: 15px;"
        "   border-color: black;"
        "   padding: 10px"
        "}"
    );
    innerFrame->setMaximumSize(200, 200);
    bgLayout->addWidget(innerFrame, 0, Qt::AlignCenter);

    // Layout
    auto* contentLayout = new QVBoxLayout(innerFrame);
    contentLayout->setContentsMargins(15, 15, 15, 15);
    contentLayout->setSpacing(10);

    auto *modelInput = new QLineEdit(this);
    contentLayout->addWidget(modelInput);

    auto *saveButton = new QPushButton("Save", this);
    contentLayout->addWidget(saveButton);

    connect(saveButton, &QPushButton::clicked, this, [this, modelInput]() {
        this->saveSettings(modelInput);
        this->hide();
        });

    this->resize(200, 300);
    this->setMaximumSize(innerFrame->sizeHint().width() + 100, innerFrame->sizeHint().height() + 100);
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
