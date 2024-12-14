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

private slots:
    void onLoginResponse();
    void onRegisterResponse();

private:
    QNetworkAccessManager* manager;
};

#endif  // HTTPCLIENT_H

