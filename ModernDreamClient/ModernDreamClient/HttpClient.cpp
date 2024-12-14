#include "HttpClient.h"
#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>



HttpClient::HttpClient(QObject* parent)
    : QObject(parent), manager(new QNetworkAccessManager(this))
{
}

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
    QJsonDocument doc(json);
    QByteArray data = doc.toJson();

    QNetworkReply* reply = manager->post(request, data);

    connect(reply, &QNetworkReply::finished, this, &HttpClient::onRegisterResponse);
}

//void HttpClient::onLoginResponse()
//{
//    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
//
//    if (reply->error() == QNetworkReply::NoError) {
//        QByteArray responseData = reply->readAll();
//        QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);
//        if (jsonDoc.isObject()) {
//            QJsonObject jsonObj = jsonDoc.object();
//            emit loginSuccess(jsonObj["username"].toString(), jsonObj["score"].toInt());
//        }
//    }
//    else {
//        emit loginFailure(reply->errorString());
//    }
//
//    reply->deleteLater();
//}

void HttpClient::onLoginResponse()
{
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());

    qDebug() << "Login Response Status:" << reply->error();
    qDebug() << "Response Body:" << reply->readAll();

    if (reply->error() == QNetworkReply::NoError) {
        QByteArray responseData = reply->readAll();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);

        if (jsonDoc.isObject()) {
            QJsonObject jsonObj = jsonDoc.object();
            qDebug() << "Username:" << jsonObj["username"].toString();
            qDebug() << "Score:" << jsonObj["score"].toInt();

            emit loginSuccess(jsonObj["username"].toString(), jsonObj["score"].toInt());
        }
        else {
            qDebug() << "Invalid JSON response";
            emit loginFailure("Invalid server response");
        }
    }
    else {
        qDebug() << "Login Error:" << reply->errorString();
        emit loginFailure(reply->errorString());
    }

    reply->deleteLater();
}

void HttpClient::onRegisterResponse()
{
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());

    if (reply->error() == QNetworkReply::NoError) {
        emit registerSuccess();
    }
    else {
        emit registerFailure(reply->errorString());
    }

    reply->deleteLater();
}
