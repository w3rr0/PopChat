#include "ollamaclient.h"

#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>

OllamaClient::OllamaClient(QObject* parent) : QObject{ parent } {
	networkManager = new QNetworkAccessManager(this);
}

void OllamaClient::resetConversation() {
	conversationHistory.clear();
	qDebug() << "Historia rozmowy wyczyszczona";
}

void OllamaClient::sendMessage(const QString &text) {
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
	// Implementation of handling read-only response from the API
}