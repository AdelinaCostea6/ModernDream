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

signals:
    void loginSuccess(const QString& username, int score);  // Signal for successful login
    void registerSuccess();  // Signal for successful registration
    void loginFailure(const QString& error);  // Signal for failed login
    void registerFailure(const QString& error);  // Signal for failed registration

private slots:
    void onLoginResponse();
    void onRegisterResponse();

private:
    QNetworkAccessManager* manager;
};

#endif  // HTTPCLIENT_H

