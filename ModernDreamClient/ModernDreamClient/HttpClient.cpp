#include "HttpClient.h"
#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>


HttpClient::HttpClient(QObject* parent)
    : QObject(parent), manager(new QNetworkAccessManager(this)) {}

void HttpClient::login(const QString& username)
{
    
    QUrl url("http://localhost:8080/login/" + username);
    QNetworkRequest request(url);

    
    QNetworkReply* reply = manager->get(request);
    connect(reply, &QNetworkReply::finished, this, &HttpClient::onLoginResponse);
}

void HttpClient::registerUser(const QString& username)
{
    
    QUrl url("http://localhost:8080/register");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject json;
    json["username"] = username;
    QByteArray data = QJsonDocument(json).toJson();

  
    QNetworkReply* reply = manager->post(request, data);
    connect(reply, &QNetworkReply::finished, this, &HttpClient::onRegisterResponse);
}

void HttpClient::onLoginResponse()
{
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    if (!reply) return;

    QByteArray responseData = reply->readAll();

    if (reply->error() != QNetworkReply::NoError) {
        // Afișează mesajul de eroare clar
        QString errorString;
        if (reply->error() == QNetworkReply::ContentNotFoundError) {
            errorString = "User not found";
        }
        else {
            errorString = reply->errorString(); 
        }
        emit loginFailure(errorString);
    }
    else {
        QJsonObject jsonObj = QJsonDocument::fromJson(responseData).object();
        if (jsonObj.contains("username") && jsonObj.contains("score")) {
            emit loginSuccess(jsonObj["username"].toString(), jsonObj["score"].toInt());
        }
        else {
            emit loginFailure("Invalid server response");
        }
    }

    reply->deleteLater();
}


void HttpClient::onRegisterResponse()
{
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    if (!reply) return;

    if (reply->error() == QNetworkReply::NoError) {
        emit registerSuccess();
    }
    else {
       
        QString errorString;
        if (reply->error() == QNetworkReply::ContentConflictError) { // Cod 409
            errorString = "Username already exists";
        }
        else {
            errorString = reply->errorString();
        }
        emit registerFailure(errorString);
    }

    reply->deleteLater();
}

void HttpClient::joinGame(const QString& username, const QString& mapType, int requiredPlayers) {
    QUrl url("http://localhost:8080/game/join");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject json;
    json["username"] = username;
    json["mapType"] = mapType;
    json["requiredPlayers"] = requiredPlayers;

    QByteArray data = QJsonDocument(json).toJson();
    QNetworkReply* reply = manager->post(request, data);
    connect(reply, &QNetworkReply::finished, this, &HttpClient::onJoinGameResponse);
}

void HttpClient::onJoinGameResponse() {
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    if (!reply) return;

    if (reply->error() == QNetworkReply::NoError) {
        QJsonObject response = QJsonDocument::fromJson(reply->readAll()).object();

        currentSessionId = response["sessionId"].toString();
        int currentPlayers = response["currentPlayers"].toInt();
        int requiredPlayers = response["requiredPlayers"].toInt();

        emit joinGameSuccess(currentSessionId, currentPlayers, requiredPlayers);

        // Start periodic status checks
        if (!statusCheckTimer) {
            statusCheckTimer = new QTimer(this);
            statusCheckTimer->setInterval(2000); // Check every 2 seconds
            connect(statusCheckTimer, &QTimer::timeout, [this]() {
                checkGameStatus(currentSessionId);
                });
        }
        statusCheckTimer->start();
    }
    else {
        emit joinGameFailure(reply->errorString());
    }

    reply->deleteLater();
}

void HttpClient::checkGameStatus(const QString& sessionId) {
    QUrl url(QString("http://localhost:8080/game/status/%1").arg(sessionId));
    QNetworkRequest request(url);

    QNetworkReply* reply = manager->get(request);
    connect(reply, &QNetworkReply::finished, this, &HttpClient::onCheckStatusResponse);
}

void HttpClient::onCheckStatusResponse() {
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    if (!reply) return;

    if (reply->error() == QNetworkReply::NoError) {
        QJsonObject response = QJsonDocument::fromJson(reply->readAll()).object();

        int currentPlayers = response["currentPlayers"].toInt();
        int requiredPlayers = response["requiredPlayers"].toInt();

        if (response["status"].toString() == "ready") {
            statusCheckTimer->stop();
            emit gameReady(currentSessionId, response["players"].toArray());
        }
        else if (response["status"].toString() == "waiting") {
            if (response.contains("lastJoined")) {
                emit playerJoined(response["lastJoined"].toString(),
                    currentPlayers, requiredPlayers);
            }
            if (response.contains("lastLeft")) {
                emit playerLeft(response["lastLeft"].toString(),
                    currentPlayers, requiredPlayers);
            }
        }
    }

    reply->deleteLater();
}