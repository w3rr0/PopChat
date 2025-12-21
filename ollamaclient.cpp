#include "ollamaclient.h"

OllamaClient::OllamaClient(QObject *parent) : QObject(parent) {
	networkManager = new QNetworkAccessManager(this);
	currentReply = nullptr;
}

void OllamaClient::sendMessage() {
	// Implementation of sending a message to the Ollama API
}

void OllamaClient::resetConversation() {
	conversationHistory.clear();
}

void OllamaClient::onReadOnly() {
	// Implementation of handling read-only response from the API
}