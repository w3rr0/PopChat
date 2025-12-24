#include "chatbubble.h"

#include <QHBoxLayout>
#include <QFontMetrics>

ChatBubble::ChatBubble(const QString &text, bool isSender, QWidget *parent)
    : QWidget(parent) {
    m_isSender = isSender;

    // Layout
    auto *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    // Frame
    frame = new QFrame(this);
    QString bg = isSender ? "#0078D7" : "#3A3A3A";
    QString color = isSender ? "white" : "#E0E0E0";
    frame->setStyleSheet(QString(
        "QFrame { background-color: %1; border-radius: 12px; }"
    ).arg(bg));

    // Inner
    auto *innerLayout = new QVBoxLayout(frame);
    innerLayout->setContentsMargins(15, 10, 15, 10);
    innerLayout->setVerticalSizeConstraint(QLayout::SetFixedSize);

    // Label
    m_label = new QLabel(text, frame);
    m_label->setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::LinksAccessibleByMouse);
    m_label->setWordWrap(true);
	m_label->setTextFormat(Qt::MarkdownText);

    QFont font = m_label->font();
    font.setPixelSize(14);
    m_label->setFont(font);
    m_label->setStyleSheet(QString("QLabel { color: %1; background-color: transparent; border: none; margin: 0px; }").arg(color));

    QSizePolicy sizePolicy(QSizePolicy::Ignored, QSizePolicy::Minimum);
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
        int limitWidth = this->width() * 0.75;
        QFontMetrics fm(m_label->font());
        int textWidth = fm.horizontalAdvance(m_label->text()) + 2;
        int finalWidth = std::min(textWidth, limitWidth);
        m_label->setFixedWidth(finalWidth);
    } else if (m_label && this->parentWidget()) {
        int maxWidth = this->parentWidget()->width() - 35;
		m_label->setFixedWidth(maxWidth);
    }
    QWidget::resizeEvent(event);
}

void ChatBubble::appendText(const QString &chunk) {
    if (m_label) {
        QString current = m_label->text();
        m_label->setText(current + chunk);

        this->adjustSize();
    }
}
