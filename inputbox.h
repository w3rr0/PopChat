#ifndef POPCHAT_INPUTBOX_H
#define POPCHAT_INPUTBOX_H

#include <QLineEdit>


class InputBox final : public QLineEdit {
    Q_OBJECT

public:
    explicit InputBox(QWidget *parent = nullptr);
    ~InputBox() override = default;
    void reloadPlaceholderText();
};


#endif //POPCHAT_INPUTBOX_H