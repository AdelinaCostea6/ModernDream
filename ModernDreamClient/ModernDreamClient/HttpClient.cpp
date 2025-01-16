#include "HttpClient.h"
#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QMutexLocker>
#include "ModernDreamClient.h"



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


   // QUrl url("http://localhost:8080/game/join");
   // QNetworkRequest request(url);
   // request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

   // QJsonObject json;
   // json["username"] = username;
   // json["mapType"] = mapType;
   // json["requiredPlayers"] = requiredPlayers;
   // json["sessionId"] = ModernDreamClient::sharedSessionId; // Folosește sesiunea partajată
   // 

   // QByteArray data = QJsonDocument(json).toJson();
   // QNetworkReply* reply = manager->post(request, data);
   //// connect(reply, &QNetworkReply::finished, this, &HttpClient::onJoinGameResponse);
   // connect(reply, &QNetworkReply::finished, this, [this, reply]() {
   //     //joiningInProgress = false;
   //     onJoinGameResponse(reply);
   //     });
}


//void HttpClient::joinGame(const QString& username, const QString& mapType, int requiredPlayers, int x, int y) {
//    if (!currentSessionId.isEmpty()) {
//        qDebug() << "Already in a session. Session ID:" << currentSessionId;
//        return;
//    }
//
//    if (joiningInProgress) {
//        qDebug() << "joinGame already in progress. Skipping...";
//        return;
//    }
//
//    joiningInProgress = true;
//    QUrl url("http://localhost:8080/game/join");
//    QNetworkRequest request(url);
//    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
//
//    QJsonObject json;
//    json["username"] = username;
//    json["mapType"] = mapType;
//    json["requiredPlayers"] = requiredPlayers;
//    json["position"] = QJsonArray({ x, y });  // Poziția jucătorului
//
//    qDebug() << "JSON Sent:" << QJsonDocument(json).toJson();
//
//    QByteArray data = QJsonDocument(json).toJson();
//    QNetworkReply* reply = manager->post(request, data);
//
//    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
//        joiningInProgress = false;
//        onJoinGameResponse(reply);
//        });
//}





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

//QByteArray HttpClient::requestMapGeneration(int numPlayers) { 
//    QUrl url("http://localhost:8080/generateMap");
//    QNetworkRequest request(url);
//    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
//
//    QJsonObject json;
//    json["numPlayers"] = numPlayers;
//
//    QByteArray data = QJsonDocument(json).toJson();
//    QNetworkReply* reply = manager->post(request, data);
//
//    QEventLoop loop;
//    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
//    loop.exec();
//
//    return reply->readAll();
//}

QByteArray HttpClient::requestMapGeneration(const QString& sessionId, int numPlayers) {
    QUrl url("http://localhost:8080/generateMap");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject json;
    json["sessionId"] = sessionId;  // Adăugăm `sessionId` pentru a identifica sesiunea
    json["numPlayers"] = numPlayers;

    QByteArray data = QJsonDocument(json).toJson();
    QNetworkReply* reply = manager->post(request, data);

    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    return reply->readAll();  // Returnează răspunsul serverului
}




void HttpClient::movePlayer(const QString& sessionId, const QString& username, const QString& direction) {
    // Crearea URL-ului pentru endpoint-ul de mișcare
    QUrl url("http://localhost:8080/game/move");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // Construim corpul JSON al cererii
    QJsonObject json;
    json["sessionId"] = sessionId;
    json["username"] = username;
    json["direction"] = direction;

    QByteArray data = QJsonDocument(json).toJson();

    // Trimite cererea `POST`
    QNetworkReply* reply = manager->post(request, data);

    connect(reply, &QNetworkReply::finished, [this, reply]() {
        QByteArray responseData = reply->readAll();
        QJsonObject response = QJsonDocument::fromJson(responseData).object();

        if (reply->error() == QNetworkReply::NoError) {
            qDebug() << "Move successful. Server response:" << response;

            // Extragem poziția nouă din răspunsul serverului
            if (response.contains("position")) {
                QJsonArray posArray = response["position"].toArray();
                int x = posArray[0].toInt();
                int y = posArray[1].toInt();

                emit playerMoved(x, y);  // Semnalizează poziția nouă către client
            }
        }
        else {
            qDebug() << "Move failed:" << reply->errorString();
        }

        reply->deleteLater();  // Curățăm răspunsul după terminare
        });
}

//
//void HttpClient::shootBullet(const QString& sessionId, const QString& username, const QString& direction) {
//    QJsonObject data;
//    data["sessionId"] = sessionId;
//    data["username"] = username;
//    data["direction"] = direction;
//    QNetworkRequest request(QUrl("http://localhost:8080/game/shoot"));
//    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
//    QNetworkReply* reply = manager->post(request, QJsonDocument(data).toJson());
//    connect(reply, &QNetworkReply::finished, [reply]() {
//        QByteArray responseData = reply->readAll();
//        qDebug() << "Răspuns server pentru glonț: " << QString(responseData);
//        reply->deleteLater();
//        });
//}

//
//void HttpClient::syncBullets(const QString& sessionId) {
//    QJsonObject data;
//    data["sessionId"] = sessionId;
//
//    QNetworkRequest request(QUrl("http://localhost:8080/game/syncBullets"));
//    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
//
//    QNetworkReply* reply = manager->post(request, QJsonDocument(data).toJson());
//    connect(reply, &QNetworkReply::finished, [this, reply]() {
//        QByteArray responseData = reply->readAll();
//        QJsonObject jsonResponse = QJsonDocument::fromJson(responseData).object();
//        QJsonArray bulletsArray = jsonResponse["bullets"].toArray();
//
//        QVector<BulletInfo> updatedBullets;
//        for (const QJsonValue& bulletValue : bulletsArray) {
//            QJsonObject bulletObj = bulletValue.toObject();
//            int x = bulletObj["x"].toInt();
//            int y = bulletObj["y"].toInt();
//            char direction = bulletObj["direction"].toString().toLatin1()[0];
//
//            if (x < 0 || y < 0) {
//                qDebug() << "Invalid bullet data from server: (" << x << ", " << y << ")";
//                continue;
//            }
//
//            updatedBullets.push_back({ x, y, direction });
//        }
//
//        emit bulletsUpdated(updatedBullets);
//        qDebug() << "Emitted bulletsUpdated signal with " << updatedBullets.size() << " bullets.";
//        reply->deleteLater();
//        });

//}





//void HttpClient::syncBullets(const QString& sessionId) {
//    QUrl url = QString("http://localhost:8080/game/syncBullets");
//    QJsonObject json;
//    json["sessionId"] = sessionId;
//
//    QNetworkRequest request(url);
//    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
//
//    QJsonDocument doc(json);
//    QNetworkReply* reply = manager->post(request, doc.toJson());
//
//    connect(reply, &QNetworkReply::finished, [this, reply]() {
//        if (reply->error() == QNetworkReply::NoError) {
//            QJsonDocument responseDoc = QJsonDocument::fromJson(reply->readAll());
//            QJsonArray bulletsArray = responseDoc.object()["bullets"].toArray();
//
//            QVector<BulletInfo> newBullets;
//            for (const auto& bulletValue : bulletsArray) {
//                QJsonObject bulletJson = bulletValue.toObject();
//                BulletInfo bullet;
//                bullet.x = bulletJson["x"].toInt();
//                bullet.y = bulletJson["y"].toInt();
//                bullet.direction = bulletJson["direction"].toString().at(0).toLatin1();
//
//                // Perform validation
//                if (bullet.x >= 0 && bullet.y >= 0) {
//                    newBullets.append(bullet);
//                }
//            }
//
//            // Safely update bullets
//            {
//                QMutexLocker lock(&bulletsMutex);
//                bullets.swap(newBullets);
//            }
//            emit bulletsUpdated(bullets);
//        }
//        else {
//            qDebug() << "Error syncing bullets:" << reply->errorString();
//        }
//        reply->deleteLater();
//        });
//}

//void HttpClient::syncBullets(const QString& sessionId) {
//    QUrl url = QString("http://localhost:8080/game/syncBullets");
//    QJsonObject json;
//    json["sessionId"] = sessionId;
//
//    QNetworkRequest request(url);
//    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
//
//    QJsonDocument doc(json);
//    QNetworkReply* reply = manager->post(request, doc.toJson());
//
//    connect(reply, &QNetworkReply::finished, [this, reply]() {
//        if (reply->error() == QNetworkReply::NoError) {
//            QJsonDocument responseDoc = QJsonDocument::fromJson(reply->readAll());
//            QJsonArray bulletsArray = responseDoc.object()["bullets"].toArray();
//
//            QVector<QPair<int, int>> bulletPositions; // To store x, y directly
//            QVector<char> bulletDirections; // To store directions directly
//
//            for (const auto& bulletValue : bulletsArray) {
//                QJsonObject bulletJson = bulletValue.toObject();
//
//                // Extract values directly
//                int x = bulletJson["x"].toInt();
//                int y = bulletJson["y"].toInt();
//                QString directionStr = bulletJson["direction"].toString();
//                char direction = directionStr.isEmpty() ? '\0' : directionStr[0].toLatin1();
//
//                // Perform validation
//                if (x >= 0 && y >= 0) {
//                    bulletPositions.append({ x, y });
//                    bulletDirections.append(direction);
//                }
//            }
//
//            // Safely update bullets
//            {
//                QMutexLocker lock(&bulletsMutex);
//                this->bulletPositions.swap(bulletPositions); // Assuming bulletPositions is defined
//                this->bulletDirections.swap(bulletDirections);
//            } 
//
//            emit bulletsUpdated(this->bulletPositions, this->bulletDirections); // Adjust emit if needed
//        }
//        else {
//            qDebug() << "Error syncing bullets:" << reply->errorString();
//        }
//        reply->deleteLater();
//        });
//}
//
//

//void HttpClient::shootBullet(const QString& sessionId, const QString& username, const QString& direction) {
//    QJsonObject requestData;
//    requestData["sessionId"] = sessionId;
//    requestData["username"] = username;
//    requestData["direction"] = direction;
//
//    QNetworkRequest request(QUrl("http://localhost:8080/game/shoot"));
//    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
//
//    QNetworkReply* reply = manager->post(request, QJsonDocument(requestData).toJson());
//    connect(reply, &QNetworkReply::finished, [this, reply]() {
//        QByteArray responseData = reply->readAll();
//
//        if (reply->error() == QNetworkReply::NoError) {
//            QJsonDocument responseDoc = QJsonDocument::fromJson(responseData);
//            QJsonObject responseObj = responseDoc.object();
//
//            QVector<QPair<int, int>> bulletPositions;
//            QJsonArray bulletsArray = responseObj["bullets"].toArray();
//            for (const auto& bullet : bulletsArray) {
//                QJsonObject bulletObj = bullet.toObject();
//                int x = bulletObj["x"].toInt();
//                int y = bulletObj["y"].toInt();
//                bulletPositions.append(qMakePair(x, y));
//            }
//
//            if (bulletPositions.isEmpty()) {
//                qDebug() << "Received empty bullet list from server!";
//                return;  // Evită emiterea semnalului dacă lista este goală
//            }
//
//            emit bulletsUpdated(bulletPositions); // Emit the updated bullet positions
//        }
//        else {
//            qDebug() << "Error shooting bullet: " << reply->errorString();
//        }
//
//        reply->deleteLater();
//        });
//}

//void HttpClient::shootBullet(const QString& sessionId, const QString& username, const QString& direction) {
//   /* QJsonObject requestData;
//    requestData["sessionId"] = sessionId;
//    requestData["username"] = username;
//    requestData["direction"] = direction;
//
//    QNetworkRequest request(QUrl("http://localhost:8080/game/shoot"));
//    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
//    qDebug() << "Adding bullet in direction:" << requestData["direction"].toString(); 
//    QNetworkReply* reply = manager->post(request, QJsonDocument(requestData).toJson());
//    connect(reply, &QNetworkReply::finished, [this, reply]() {
//        reply->deleteLater();
//        
//        });*/
//
//    if (sessionId.isEmpty() || username.isEmpty()) {
//        qDebug() << "Error: sessionId or username is empty!";
//        return;
//    }
//
//    // Construiește datele cererii
//    QJsonObject requestData;
//    requestData["sessionId"] = sessionId;
//    requestData["username"] = username;
//    requestData["direction"] = direction;
//
//    QNetworkRequest request(QUrl("http://localhost:8080/game/shoot"));
//    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
//
//    // Trimite cererea POST
//    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
//    QNetworkReply* reply = manager->post(request, QJsonDocument(requestData).toJson());
//
//    connect(reply, &QNetworkReply::finished, [this, reply, direction]() {
//        QByteArray responseData = reply->readAll();
//        qDebug() << "Server response for shootBullet:" << responseData;
//
//        // Prelucrează răspunsul serverului dacă este necesar
//        auto jsonResponse = QJsonDocument::fromJson(responseData).object();
//        if (jsonResponse.contains("startX") && jsonResponse.contains("startY")) {
//            int startX = jsonResponse["startX"].toInt();
//            int startY = jsonResponse["startY"].toInt();
//
//            // Adaugă glonțul local pentru deplasare
//            QMutexLocker lock(&bulletsMutex);
//            bullets->append(BulletInfo(startX, startY, direction[0].toLatin1()));
//            qDebug() << "Added bullet locally at: (" << startY << ", " << startX << ")";
//        }
//        else {
//            qDebug() << "Invalid response from server for shootBullet!";
//        }
//
//        //update();  // Re-desenare
//        reply->deleteLater();
//        });
//}

//void HttpClient::syncBullets(const QString& sessionId) {
//    QJsonObject data;
//    data["sessionId"] = sessionId;
//
//    QNetworkRequest request(QUrl("http://localhost:8080/game/syncBullets"));
//    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
//
//    QNetworkReply* reply = manager->post(request, QJsonDocument(data).toJson());
//    connect(reply, &QNetworkReply::finished, [this, reply]() {
//        QByteArray responseData = reply->readAll();
//        QJsonObject jsonResponse = QJsonDocument::fromJson(responseData).object();
//        QJsonArray bulletsArray = jsonResponse["bullets"].toArray();
//
//        qDebug() << "Server response JSON:" << QString::fromUtf8(responseData);
//
//        /*QVector<QPair<int, int>> bulletPositions;
//        for (const QJsonValue& bulletValue : bulletsArray) {
//            QJsonObject bulletObj = bulletValue.toObject();
//            int x = bulletObj["x"].toInt();
//            int y = bulletObj["y"].toInt();
//            bulletPositions.push_back(qMakePair(x, y));
//        }*/
//        auto newBullets = QSharedPointer<QVector<BulletInfo>>::create();
//        for (const QJsonValue& bulletValue : bulletsArray) {
//            QJsonObject bulletObj = bulletValue.toObject();
//            int x = bulletObj["x"].toInt();
//            int y = bulletObj["y"].toInt();
//            qDebug() << "Updating bullet to: (" << x << ", " << y << ")";
//            /*{
//                QMutexLocker lock(&bulletsMutex);
//                bullets = newBullets;
//            }*/
//            newBullets->append(BulletInfo(x, y));  
//        }
//        
//
//        // emit bulletsUpdated(bulletPositions);
//        emit bulletsUpdated(*newBullets);
//        
//        reply->deleteLater();
//        });
//}







