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

