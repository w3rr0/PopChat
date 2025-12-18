#include "chatbubble.h"

#include <QHBoxLayout>
#include <QFontMetrics>

ChatBubble::ChatBubble(const QString &text, bool isSender, QWidget *parent)
    : QWidget(parent) {
    m_isSender = isSender;

    // Layout
    auto *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSizeConstraint(QLayout::SetMinAndMaxSize);

    auto *frame = new QFrame(this);
    QString bg = isSender ? "#0078D7" : "#3A3A3A";
    QString color = isSender ? "white" : "#E0E0E0";
    frame->setStyleSheet(QString(
        "QFrame { background-color: %1; border-radius: 12px; }"
    ).arg(bg));

    auto *innerLayout = new QVBoxLayout(frame);
    innerLayout->setContentsMargins(15, 10, 15, 10);
    innerLayout->setSizeConstraint(QLayout::SetFixedSize);

    // Label
    m_label = new QLabel(text, frame);
    m_label->setTextInteractionFlags(Qt::TextSelectableByMouse);
    m_label->setWordWrap(true);

    QFont font = m_label->font();
    font.setPixelSize(14);
    m_label->setFont(font);
    m_label->setStyleSheet(QString("QLabel { color: %1; background-color: transparent; border: none; }").arg(color));

    // Size Policy
    QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
    m_label->setSizePolicy(sizePolicy);

    innerLayout->addWidget(m_label);

    // Styling
    if (isSender) {
        layout->addStretch();
        layout->addWidget(frame);
    } else {
        layout->addWidget(frame);
    }
}

void ChatBubble::resizeEvent(QResizeEvent *event) {
    if (m_label && m_isSender) {
        m_label->setMaximumWidth(this->width() * 0.8);
    }
    QWidget::resizeEvent(event);
}
