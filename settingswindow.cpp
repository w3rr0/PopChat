#include "settingswindow.h"
#include <QtWidgets/qboxlayout.h>
#include "ollamaclient.h"
#include "theme.h"

#include <QPushButton>
#include <QSettings>
#include <QLineEdit>
#include <QLabel>
#include <QComboBox>
#include <QScreen>

SettingsWindow::SettingsWindow(OllamaClient *client, QWidget *parent)
    : QDialog(parent), client(client)
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
    bgLayout->addWidget(innerFrame, 0, Qt::AlignCenter);

    // Layout
    auto* contentLayout = new QVBoxLayout(innerFrame);
    contentLayout->setContentsMargins(15, 15, 15, 15);
    contentLayout->setSpacing(10);

    auto *settingFrame = new QFrame(innerFrame);
    settingFrame->setContentsMargins(0, 0, 0, 0);
    settingFrame->setFixedWidth(270);
    auto *settingLayout = new QHBoxLayout(settingFrame);
    settingLayout->setContentsMargins(0, 0, 0, 0);
    settingLayout->setSpacing(10);
    settingLayout->setSizeConstraint(QLayout::SetMaximumSize);

    auto *label = new QLabel("Model Name:", settingFrame);
    label->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
    label->setStyleSheet(QString(
        "padding: 0px;"
        "color: %1;"
    ).arg(Theme::TextUser));
    settingLayout->addWidget(label, 0);

    modelInput = new QComboBox(settingFrame);
    modelInput->setStyleSheet(QString(
        "QComboBox {"
        "   background-color: %1;"
        "   color: white;"
        "   border: 1px solid #555;"
        "   border-radius: 5px;"
        "   padding: 5px;"
        "}"
        "QComboBox::drop-down { border: none; }"
        "QComboBox QAbstractItemView {"
        "   background-color: %2;"
        "   selection-background-color: %3;"
        "   color: white;"
        "}"
    ).arg(Theme::WindowBg, Theme::Surface, Theme::Accent));
    settingLayout->addWidget(modelInput, 1);

    client->fetchModels();

    contentLayout->addWidget(settingFrame, 1, Qt::AlignCenter);

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
    ).arg(Theme::Accent, Theme::AccentHover, Theme::AccentPressed, Theme::TextMain));
    contentLayout->addWidget(saveButton);

    connect(saveButton, &QPushButton::clicked, this, [this]() {
        if (const QString selectedModel = modelInput->currentText().trimmed(); selectedModel != OllamaClient::getModelName()) {
            saveSettings(selectedModel);
            emit modelChanged();
        }
        this->hide();
        });

    warning->setStyleSheet(QString(
        "color: %1;"
        ).arg(Theme::Warning));
    contentLayout->addWidget(warning);
    warning->hide();

    connect(client, &OllamaClient::errorOccurs, this, [this, innerFrame](const QString &errorMessage) {
        warning->setText(errorMessage);
        warning->show();
        this->changeSize(*innerFrame);
    });
    connect(client, &OllamaClient::modelsReceived, this, [this, innerFrame](const QStringList& models) {
        modelInput->clear();
        modelInput->addItems(models);

        if (const auto currentModel = OllamaClient::getModelName(); currentModel.isEmpty() && !models.isEmpty()) {
            const QString &defaultModel = models.first();
			modelInput->setCurrentText(defaultModel);
            this->saveSettings(defaultModel);
            emit modelChanged();
        } else if (!currentModel.isEmpty()) {
            modelInput->setCurrentText(currentModel);
        }

        warning->hide();
        this->changeSize(*innerFrame);
        });

    this->changeSize(*innerFrame);

    const QScreen *screen = QGuiApplication::primaryScreen();
    const QRect screenGeometry = screen->availableGeometry();

    const int x = (screenGeometry.width() - this->width()) / 2;
    const int y = (screenGeometry.height() - this->height()) / 2;

    this->move(x, y);
}

void SettingsWindow::saveSettings(const QString &selectedModel) {
    QSettings settings("model");

    if (selectedModel.isEmpty()) {
        return;
    }

    OllamaClient::setModelName(selectedModel);
    settings.setValue("modelName", selectedModel);

    settings.sync();
}

void SettingsWindow::changeSize(const QFrame &innerFrame) {
    this->setMaximumSize(innerFrame.sizeHint().width() + 100, innerFrame.sizeHint().height() + 100);
    this->resize(this->maximumSize());
}
