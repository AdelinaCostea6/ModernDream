#pragma once
#ifndef HTTPCLIENT_H
#define HTTPCLIENT_H

#include <QObject>
#include <QtNetwork>
#include <QJsonDocument>
#include <QJsonObject>

class HttpClient : public QObject
{
    Q_OBJECT

public:
    explicit HttpClient(QObject* parent = nullptr);

    void login(const QString& username);
    void registerUser(const QString& username);
    void createGame(int requiredPlayers);
    void joinGame(const QString& username, const QString& mapType, int requiredPlayers);
    void checkGameStatus(const QString& sessionId);
    void leaveGame(const QString& sessionId);

signals:
    void loginSuccess(const QString& username, int score);  
    void registerSuccess();  
    void loginFailure(const QString& error);  
    void registerFailure(const QString& error);  

    void joinGameSuccess(const QString& sessionId, int currentPlayers, int requiredPlayers);
    void joinGameFailure(const QString& error);
    void gameReady(const QString& sessionId, const QJsonArray& players);
    void playerJoined(const QString& username, int currentPlayers, int requiredPlayers);
    void playerLeft(const QString& username, int currentPlayers, int requiredPlayers);


private slots:
    void onLoginResponse();
    void onRegisterResponse();
    void onCreateGameResponse();

    void onJoinGameResponse(QNetworkReply* reply);
    void onCheckStatusResponse();
    void onLeaveGameResponse();

private:
    QNetworkAccessManager* manager;

    QTimer* statusCheckTimer; 
    QString currentSessionId; 
    bool joiningInProgress = false;
};

#endif  // HTTPCLIENT_H

