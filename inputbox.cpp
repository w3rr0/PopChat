#include "inputbox.h"

#include "ollamaclient.h"
#include "settingswindow.h"
#include "theme.h"

InputBox::InputBox(QWidget *parent)
    : QLineEdit(parent) {

    this->setStyleSheet(QString(
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
    this->setAttribute(Qt::WA_MacShowFocusRect, false);
#endif

    this->reloadPlaceholderText();
}

void InputBox::reloadPlaceholderText() {
    this->setPlaceholderText(!OllamaClient::getModelName().isEmpty() ? "Ask..." : "Select model first");
}