#include "ollamaclient.h"

#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>

QString OllamaClient::modelName = "";

OllamaClient::OllamaClient(QObject* parent) : QObject{ parent } {
	networkManager = new QNetworkAccessManager(this);
}

void OllamaClient::resetConversation() {
	conversationHistory.clear();
}

void OllamaClient::sendMessage(const QString &text) {
	if (text.isEmpty()) {
		return;
	}
	
	if (modelName.isEmpty()) {
		emit textReceived("[Model not selected]");
		return;
	}

	ChatMessage userMsg;
	userMsg.role = "user";
	userMsg.content = text;
	conversationHistory.append(userMsg);

	QJsonArray messagesArray;
	for (const auto& msg : conversationHistory) {
		QJsonObject msgObject;
		msgObject["role"] = msg.role;
		msgObject["content"] = msg.content;
		messagesArray.append(msgObject);
	}

	// Build request object
	QJsonObject root;
	root["model"] = modelName;
	root["messages"] = messagesArray;
	root["stream"] = true;

	QJsonDocument doc(root);
	QByteArray data = doc.toJson();

	// HTTP request config
	QNetworkRequest request((QUrl(apiUrl)));
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

	// Send request
	currentReply = networkManager->post(request, data);

	// Clear buffer
	generatedBuffer.clear();

	// Signals connections
	connect(currentReply, &QNetworkReply::readyRead, this, &OllamaClient::onReadyRead);
	connect(currentReply, &QNetworkReply::errorOccurred, this, [this](QNetworkReply::NetworkError code) {
		qDebug() << "Network error: " << code << currentReply->errorString();
		emit textReceived("\n[Cannot connect to ollama, check whether it is running]");
		currentReply->deleteLater();
		currentReply = nullptr;
		});
}

void OllamaClient::onReadyRead() {
	while (currentReply && currentReply->canReadLine()) {
		QByteArray line = currentReply->readLine();

		auto doc = QJsonDocument::fromJson(line);
		QJsonObject obj = doc.object();

		if (obj["done"].toBool()) {
			ChatMessage assistantMsg;
			assistantMsg.role = "assistant";
			assistantMsg.content = generatedBuffer;
			conversationHistory.append(assistantMsg);

			emit replyFinished();
			currentReply->deleteLater();
			currentReply = nullptr;
			return;
		}

		if (obj.contains("message")) {
			QJsonObject messageObj = obj["message"].toObject();
			if (messageObj.contains("content")) {
				QString token = messageObj["content"].toString();

				if (!token.isEmpty()) {
					emit textReceived(token);
					generatedBuffer += token;
				}
			}
		}
	}
}

void OllamaClient::setModelName(const QString &name) {
	modelName = name.trimmed();
}

QString OllamaClient::getModelName() {
	return modelName;
}