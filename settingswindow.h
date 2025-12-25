#ifndef POPCHAT_SETTINGSWINDOW_H
#define POPCHAT_SETTINGSWINDOW_H

#include <QDialog>


QT_BEGIN_NAMESPACE
class QLabel;
class QLineEdit;
class QWidget;
class OllamaClient;
class QComboBox;
QT_END_NAMESPACE

class SettingsWindow final : public QDialog {
    Q_OBJECT

public:
    explicit SettingsWindow(OllamaClient *client, QWidget *parent = nullptr);
    ~SettingsWindow() override = default;

signals:
    void modelChanged();

private:
    static void saveSettings(const QString &selectedModel);
    QComboBox *modelInput;
    OllamaClient *client;
};


#endif //POPCHAT_SETTINGSWINDOW_H