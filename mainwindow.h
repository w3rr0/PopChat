#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <memory>

#include "ollamaclient.h"
#include "chatbubble.h"

class QHotkey;
class OllamaClient;
class ChatBubble;
class SettingsWindow;
class QScrollArea;
class QLineEdit;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow final : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private:
    Ui::MainWindow *ui;
    QHotkey *hotkey;
    OllamaClient *client;
    ChatBubble *currentAnswerBubble;
    std::unique_ptr<SettingsWindow> settingsWindow;

    void fixPosition(QWidget *scrollContent, QScrollArea *scrollArea);
    void popWindow(QLineEdit *inputBox);
    int calculateHeight(QWidget *scrollContent, const QScrollArea *scrollArea) const;
};
#endif // MAINWINDOW_H
