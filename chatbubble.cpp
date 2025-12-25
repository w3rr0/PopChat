#include "chatbubble.h"
#include "theme.h"

#include <QHBoxLayout>
#include <QFontMetrics>

ChatBubble::ChatBubble(const QString &text, const bool isSender, QWidget *parent)
    : QWidget(parent) {
    m_isSender = isSender;

    // Layout
    auto *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    // Frame
    frame = new QFrame(this);
    const QString bg = isSender ? Theme::Accent : Theme::Response;
    const QString color = isSender ? Theme::TextMain : Theme::TextUser;
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

    constexpr QSizePolicy sizePolicy(QSizePolicy::Ignored, QSizePolicy::Minimum);
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
        const int limitWidth = this->width() * 0.75;
        const QFontMetrics fm(m_label->font());
        const int textWidth = fm.horizontalAdvance(m_label->text()) + 2;
        const int finalWidth = std::min(textWidth, limitWidth);
        m_label->setFixedWidth(finalWidth);
    } else if (m_label && this->parentWidget()) {
        const int maxWidth = this->parentWidget()->width() - 35;
		m_label->setFixedWidth(maxWidth);
    }
    QWidget::resizeEvent(event);
}

void ChatBubble::appendText(const QString &chunk) {
    if (m_label) {
        const QString current = m_label->text();
        m_label->setText(current + chunk);

        this->adjustSize();
    }
}
