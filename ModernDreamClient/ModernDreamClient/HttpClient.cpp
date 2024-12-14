#include "HttpClient.h"
#include <QDebug>

HttpClient::HttpClient(QObject* parent)
    : QObject(parent), manager(new QNetworkAccessManager(this))
{
}

void HttpClient::login(const QString& username)
{
    QUrl url("http://127.0.0.1:8080/login/" + username);
    QNetworkRequest request(url);
    QNetworkReply* reply = manager->get(request);

    connect(reply, &QNetworkReply::finished, this, &HttpClient::onLoginResponse);
}

void HttpClient::registerUser(const QString& username)
{
    QUrl url("http://127.0.0.1:8080/register");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject json;
    json["username"] = username;
    QJsonDocument doc(json);
    QByteArray data = doc.toJson();

    QNetworkReply* reply = manager->post(request, data);

    connect(reply, &QNetworkReply::finished, this, &HttpClient::onRegisterResponse);
}

void HttpClient::onLoginResponse()
{
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());

    if (reply->error() == QNetworkReply::NoError) {
        QByteArray responseData = reply->readAll();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);
        if (jsonDoc.isObject()) {
            QJsonObject jsonObj = jsonDoc.object();
            qDebug() << "Login successful:";
            qDebug() << "Username:" << jsonObj["username"].toString();
            qDebug() << "Score:" << jsonObj["score"].toInt();
        }
    }
    else 
    {
        qDebug() << "Login failed:" << reply->errorString();
    }

    reply->deleteLater();
}

void HttpClient::onRegisterResponse()
{
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());

    if (reply->error() == QNetworkReply::NoError) {
        qDebug() << "User registered successfully.";
    }
    else {
        qDebug() << "Registration failed:" << reply->errorString();
    }

    reply->deleteLater();
}
