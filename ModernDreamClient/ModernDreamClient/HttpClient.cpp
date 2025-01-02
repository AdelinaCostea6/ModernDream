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


void HttpClient::createGame(int requiredPlayers) {
    QUrl url("http://localhost:8080/game/create");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject json;
    json["requiredPlayers"] = requiredPlayers;

    QByteArray data = QJsonDocument(json).toJson();
    QNetworkReply* reply = manager->post(request, data);
    connect(reply, &QNetworkReply::finished, this, &HttpClient::onCreateGameResponse);
}

void HttpClient::onCreateGameResponse() {
  
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    if (!reply) return;

    if (reply->error() == QNetworkReply::NoError) {
        QJsonObject response = QJsonDocument::fromJson(reply->readAll()).object();

        if (response.contains("sessionId")) {
            currentSessionId = response["sessionId"].toString();
            qDebug() << "Game created successfully, session ID:" << currentSessionId;

            emit joinGameSuccess(currentSessionId, 1, response["requiredPlayers"].toInt());
        }
        else {
            qDebug() << "Error: sessionId missing in createGame response";
        }
    }
    else {
        qDebug() << "Failed to create game:" << reply->errorString();
    }
    reply->deleteLater();
}


void HttpClient::joinGame(const QString& username, const QString& mapType, int requiredPlayers) {
    if (!currentSessionId.isEmpty()) {
        qDebug() << "Already in a session. Session ID:" << currentSessionId;
        return;
    }

    if (joiningInProgress) {
        qDebug() << "joinGame already in progress. Skipping...";
        return;
    }

    joiningInProgress = true;
    QUrl url("http://localhost:8080/game/join");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject json;
    json["username"] = username;
    json["mapType"] = mapType;
    json["requiredPlayers"] = requiredPlayers;

    
    qDebug() << "JSON Sent:" << QJsonDocument(json).toJson();

    QByteArray data = QJsonDocument(json).toJson();
    QNetworkReply* reply = manager->post(request, data);
    static int connectionCount = 0;
    connectionCount++;
    qDebug() << "Connecting finished signal. Connection count:" << connectionCount;

    
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        joiningInProgress = false;  
        onJoinGameResponse(reply);
        });
}




void HttpClient::onJoinGameResponse(QNetworkReply* reply) {
if (!reply) return;

QByteArray responseData = reply->readAll();
QJsonObject response = QJsonDocument::fromJson(responseData).object();

if (reply->error() == QNetworkReply::NoError) {
    if (response.contains("sessionId")) {
        currentSessionId = response["sessionId"].toString();
        int currentPlayers = response["currentPlayers"].toInt();
        int requiredPlayers = response["requiredPlayers"].toInt();

        qDebug() << "Emitting joinGameSuccess with sessionId:" << currentSessionId
            << ", currentPlayers:" << currentPlayers
            << ", requiredPlayers:" << requiredPlayers;

        if (!statusCheckTimer) {
            statusCheckTimer = new QTimer(this);
            statusCheckTimer->setInterval(4000); 
            connect(statusCheckTimer, &QTimer::timeout, [this]() {
                checkGameStatus(currentSessionId);
                });
        }
        statusCheckTimer->start();

        emit joinGameSuccess(currentSessionId, currentPlayers, requiredPlayers);
    }
    else {
        qDebug() << "Error: sessionId missing in server response.";
    }
}
else {
    qDebug() << "Join game failed:" << reply->errorString();
    emit joinGameFailure(reply->errorString());
}

reply->deleteLater();
}




void HttpClient::leaveGame(const QString& sessionId) {
    QUrl url("http://localhost:8080/game/leave");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject json;
    json["sessionId"] = sessionId;

    QByteArray data = QJsonDocument(json).toJson();
    QNetworkReply* reply = manager->post(request, data);
    connect(reply, &QNetworkReply::finished, this, &HttpClient::onLeaveGameResponse);
}

void HttpClient::onLeaveGameResponse() {
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    if (!reply) return;

    if (reply->error() == QNetworkReply::NoError) {
        qDebug() << "Left game successfully.";
    }
    else {
        qDebug() << "Error leaving game:" << reply->errorString();
    }
    reply->deleteLater();
}

void HttpClient::checkGameStatus(const QString& sessionId) {
    if (sessionId.isEmpty()) {
        qDebug() << "Session ID is empty, cannot check status.";
        return;
    }

    QUrl url(QString("http://localhost:8080/game/status/%1").arg(sessionId));
    QNetworkRequest request(url);

    QNetworkReply* reply = manager->get(request);
    connect(reply, &QNetworkReply::finished, this, &HttpClient::onCheckStatusResponse);
}


void HttpClient::onCheckStatusResponse() {
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    if (!reply) return;

    QByteArray responseData = reply->readAll();
    qDebug() << "Status response:" << responseData; 

    QJsonObject response = QJsonDocument::fromJson(responseData).object();

    int currentPlayers = response["currentPlayers"].toInt();
    int requiredPlayers = response["requiredPlayers"].toInt();

    QString status = response["status"].toString();
    qDebug() << "Game status:" << status;  

    if (status == "ready") {
        qDebug() << "Emitting gameReady signal. Session ID:" << currentSessionId;

        
        if (statusCheckTimer) {
            statusCheckTimer->stop();
            qDebug() << "Stopped statusCheckTimer as the game is ready.";
        }

        emit gameReady(currentSessionId, response["players"].toArray());
    }

    else if (status == "waiting") {
        if (response.contains("lastJoined")) {
            QString lastJoined = response["lastJoined"].toString();
            qDebug() << "Player joined:" << lastJoined;
            emit playerJoined(lastJoined, currentPlayers, requiredPlayers);
        }
        if (response.contains("lastLeft")) {
            QString lastLeft = response["lastLeft"].toString();
            qDebug() << "Player left:" << lastLeft;
            emit playerLeft(lastLeft, currentPlayers, requiredPlayers);
        }
    }
    reply->deleteLater();
}
