#include "settingswindow.h"
#include "ollamaclient.h"

#include <QPushButton>
#include <QSettings>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QGuiApplication>

SettingsWindow::SettingsWindow(QWidget *parent)
    : QDialog(parent)
{
    this->setWindowTitle("Settings");
}

