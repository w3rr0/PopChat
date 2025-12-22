#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QHotkey>
#include <QLineEdit>
#include <QScrollArea>

#include "ollamaclient.h"
#include "chatbubble.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QHotkey *hotkey;

    OllamaClient *client;
    ChatBubble *currentAnswerBubble;

    void fixPosition(QWidget *scrollContent, QScrollArea *scrollArea);
    void popWindow(QLineEdit *inputBox);
    int calculateHeight(QWidget *scrollContent, const QScrollArea *scrollArea) const;
};
#endif // MAINWINDOW_H
