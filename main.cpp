#include "mainwindow.h"
#include "ollamaclient.h"

#include <QApplication>
#include <QLabel>
#include <QSettings>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Load Settings
    const QSettings settings("model");
    const QString savedModel = settings.value("modelName", "").toString();
    OllamaClient::setModelName(savedModel);

    // Creating main window
    MainWindow w;

    w.show();   // Ultimately hidden at the start
    return a.exec();
}
