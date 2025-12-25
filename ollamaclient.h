#ifndef POPCHAT_OLLAMACLIENT_H
#define POPCHAT_OLLAMACLIENT_H

#include <QObject>
#include <QUrl>

class QString;
class QNetworkReply;
class QNetworkAccessManager;

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
    void fetchModels();

signals:
    void textReceived(const QString &text);
    void replyFinished();
    void modelsReceived(const QStringList &models);

private slots:
    void onReadyRead();

private:
    QNetworkAccessManager *networkManager;
    QNetworkReply *currentReply = nullptr;
    QVector<ChatMessage> conversationHistory;

    inline static QString modelName = "";
    const QString apiUrl = "http://localhost:11434/api/chat";
    const QUrl modelsUrl = QUrl("http://localhost:11434/api/tags");
    QString generatedBuffer;
};


#endif //POPCHAT_OLLAMACLIENT_H