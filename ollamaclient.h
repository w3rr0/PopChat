#ifndef POPCHAT_OLLAMACLIENT_H
#define POPCHAT_OLLAMACLIENT_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QVector>
#include <QString>


struct ChatMessage {
    QString role;
    QString content;
};

class OllamaClient : public QObject {
    Q_OBJECT
public:
    explicit OllamaClient(QObject *parent = nullptr);
    void sendMessage(const QString &text);
    void resetConversation();
    static void setModelName(const QString& name);
    static QString getModelName();

signals:
    void textReceived(const QString &text);
    void replyFinished();

private slots:
    void onReadyRead();

private:
    QNetworkAccessManager *networkManager;
    QNetworkReply *currentReply;
    QVector<ChatMessage> conversationHistory;

    static QString modelName;
    const QString apiUrl = "http://localhost:11434/api/chat";
    QString generatedBuffer;
};


#endif //POPCHAT_OLLAMACLIENT_H