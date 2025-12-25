#include "ollamaclient.h"

#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <QNetworkReply>

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
	for (const auto&[role, content] : conversationHistory) {
		QJsonObject msgObject;
		msgObject["role"] = role;
		msgObject["content"] = content;
		messagesArray.append(msgObject);
	}

	// Build request object
	QJsonObject root;
	root["model"] = modelName;
	root["messages"] = messagesArray;
	root["stream"] = true;

	const QJsonDocument doc(root);
	const QByteArray data = doc.toJson();

	// HTTP request config
	QNetworkRequest request((QUrl(apiUrl)));
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

	// Send request
	currentReply = networkManager->post(request, data);

	// Clear buffer
	generatedBuffer.clear();

	// Signals connections
	connect(currentReply, &QNetworkReply::readyRead, this, &OllamaClient::onReadyRead);
	connect(currentReply, &QNetworkReply::errorOccurred, this, [this](const QNetworkReply::NetworkError code) {
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
			if (QJsonObject messageObj = obj["message"].toObject(); messageObj.contains("content")) {
				if (QString token = messageObj["content"].toString(); !token.isEmpty()) {
					emit textReceived(token);
					generatedBuffer += token;
				}
			}
		}
	}
}

void OllamaClient::setModelName(const QString &name) {
	if (const auto newName = name.trimmed(); newName != modelName) {
		modelName = newName;
	}
}

QString OllamaClient::getModelName() {
	return modelName;
}

void OllamaClient::fetchModels() {
	const QNetworkRequest request(modelsUrl);

	QNetworkReply *reply = networkManager->get(request);

	connect(reply, &QNetworkReply::finished, this, [this, reply]() {
		if (reply->error() == QNetworkReply::NoError) {
			const QByteArray data = reply->readAll();
			const QJsonDocument doc = QJsonDocument::fromJson(data);
			QJsonObject root = doc.object();

			QJsonArray modelsArray = root["models"].toArray();
			QStringList modelNames;

			for (const QJsonValue &value : modelsArray) {
				QJsonObject obj = value.toObject();
				modelNames.append(obj["name"].toString());
			}

			emit modelsReceived(modelNames);
		} else if (reply->error() == QNetworkReply::ConnectionRefusedError) {
			qDebug() << "Connection refused";	// Ollama not installed
		} else {
			qDebug() << "Error fetching models:" << reply->errorString();
			qDebug() << reply->error();
		}
		reply->deleteLater();
	});
}