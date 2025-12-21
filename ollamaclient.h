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
    void sendMessage();
    void resetConversation();
signals:
    void textReceived();
    void replyFinished();
private slots:
    void onReadOnly();
private:
    QNetworkAccessManager *networkManager;
    QNetworkReply *currentReply;
    QVector<ChatMessage> conversationHistory;

    const QString moedelName = "llama3";
    const QString apiUrl = "http://localhost:11434/api/chat";
    QString generatedBuffer;
};


#endif //POPCHAT_OLLAMACLIENT_H