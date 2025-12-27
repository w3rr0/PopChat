#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMouseEvent>
#include <QPoint>

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

protected:
	void mousePressEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
    bool nativeEvent(const QByteArray& eventType, void* message, qintptr* result);

private:
    Ui::MainWindow *ui;
    QHotkey *hotkey;
    OllamaClient *client;
    ChatBubble *currentAnswerBubble;
    SettingsWindow *settingsWindow = nullptr;
    QPoint dragPosition;
    const int borderWidth = 5;

    void fixPosition(QWidget *scrollContent, QScrollArea *scrollArea);
    void popWindow(QLineEdit *inputBox);
    int calculateHeight(QWidget *scrollContent, const QScrollArea *scrollArea) const;
};
#endif // MAINWINDOW_H
