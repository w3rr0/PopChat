#ifndef CHATBUBBLE_H
#define CHATBUBBLE_H

#include <QLabel>

class ChatBubble : public QWidget
{
    Q_OBJECT
public:
    explicit ChatBubble(const QString &text, bool isSender = true, QWidget *parent = nullptr);

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    QLabel *m_label;
    QFrame *frame;
    bool m_isSender;
};

#endif // CHATBUBBLE_H
