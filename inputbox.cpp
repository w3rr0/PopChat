#include "inputbox.h"

#include "ollamaclient.h"
#include "settingswindow.h"
#include "theme.h"

#include <qstyle.h>

InputBox::InputBox(QWidget *parent)
    : QLineEdit(parent) {

    this->setStyleSheet(QString(
        "QLineEdit {"
        "   background-color: %1;"
        "   border: 1px solid #555;"
        "   border-radius: 8px;"
        "   padding: 5px;"
        "   font-size: 14px;"
        "   color: white;"
        "}"
        "QLineEdit[isEmpty=\"true\"] {"
        "   color: %3;"
        "   border: 1px solid %3;"
        "}"
        "QLineEdit:focus { border: 1px solid %2; }"
        "QLineEdit[isEmpty=\"true\"]:focus { border: 1px solid %3;}"
    ).arg(Theme::WindowBg).arg(Theme::Accent).arg(Theme::Warning));

#ifdef Q_OS_MAC
    this->setAttribute(Qt::WA_MacShowFocusRect, false);
#endif

    this->reloadStyle();
}

void InputBox::reloadStyle() {
    const bool isEmpty = OllamaClient::getModelName().isEmpty();
    if (isEmpty) {
        this->setPlaceholderText("Select model first");
        this->setEnabled(false);
    } else {
        this->setPlaceholderText("Ask...");
        this->setEnabled(true);
    }
    this->setProperty("isEmpty", isEmpty);

    this->style()->unpolish(this);
    this->style()->polish(this);
}