#ifndef POPCHAT_SETTINGSWINDOW_H
#define POPCHAT_SETTINGSWINDOW_H

#include <QDialog>


QT_BEGIN_NAMESPACE
class QLabel;
class QLineEdit;
class QWidget;
QT_END_NAMESPACE

class SettingsWindow final : public QDialog {
    Q_OBJECT

public:
    explicit SettingsWindow(QWidget *parent = nullptr);
    ~SettingsWindow() override = default;

private:
    void saveSettings(QLineEdit *modelInput);
};


#endif //POPCHAT_SETTINGSWINDOW_H