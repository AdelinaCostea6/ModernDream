#include "GameMapWidget.h"
#include <QPainter>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <QtGlobal> 

GameMapWidget::GameMapWidget(const QString& sessionId, const QString& username, QWidget* parent)
    : QMainWindow(parent), sessionId(sessionId), username(username),transform() {
    setWindowTitle("Game Map");
    resize(1400, 800);
    bullets = QSharedPointer<QVector<BulletInfo>>::create();
    
   
    httpClient = new HttpClient(this);

    setupTextures();
    setupConnections();
    fetchAndInitializeMap();
}

void GameMapWidget::setupTextures() {
    wallTexture.load("../ModernDreamImages/tire1.png");
    bombTexture.load("../ModernDreamImages/tire1.png");
    bulletTexture.load("../ModernDreamImages/bullet2.png");


    playerTextures.resize(4);
    playerTextures[0].load("../ModernDreamImages/carBlueRight.png");
    playerTextures[1].load("../ModernDreamImages/carGreenRight.png");
    playerTextures[2].load("../ModernDreamImages/carPinkRight.png");
    playerTextures[3].load("../ModernDreamImages/carYellowRight.png");

    for (auto& texture : playerTextures) {
        texture = texture.scaled(40, 40, Qt::KeepAspectRatio);
    }
}

void GameMapWidget::setupConnections() {
   // connect(httpClient, &HttpClient::playerMoved, this, &GameMapWidget::updatePlayerPosition);
    connect(httpClient, &HttpClient::syncPlayersRequest, this, &GameMapWidget::syncPlayers);

    QTimer* bulletSyncTimer = new QTimer(this); 
    connect(bulletSyncTimer, &QTimer::timeout, this, [this]() {   
        /*if (!isUpdating)
        {*/
            syncBullets(sessionId);
            //fetchAndInitializeMap();
            //syncMap();
            updateWalls();
            syncPlayers();
            displayMap();
        //}
           
           
        });
    bulletSyncTimer->start(500);  
}

void GameMapWidget::fetchAndInitializeMap() {
   
    mapData.clear();
    qDebug() << "[DEBUG] Fetching map data...";

    QByteArray response = httpClient->requestMapGeneration(sessionId,2);
        QJsonDocument jsonDoc = QJsonDocument::fromJson(response);
        QJsonObject jsonObj = jsonDoc.object();
    
        if (jsonObj.contains("map")) {
            QJsonArray mapArray = jsonObj["map"].toArray();
            //QVector<QVector<int>> mapData;
           // mapData.clear();
            for (const QJsonValue& row : mapArray) {
                QVector<int> rowData;
                QJsonArray rowArray = row.toArray();
                for (const QJsonValue& cell : rowArray) {
                    rowData.push_back(cell.toInt());
                }
                mapData.push_back(rowData);
            }
        }
         update(); 
}


void GameMapWidget::updatePlayerPosition(int x, int y) {
    qDebug() << "Actualizare poziție jucător la: (" << x << ", " << y << ")";
    for (int i = 0; i < mapData.size(); ++i) {
        for (int j = 0; j < mapData[i].size(); ++j) {
            if (mapData[i][j] == 0) { 
                mapData[i][j] = 1;  
            }
        }
    }

    mapData[x][y] = 0; 
    update();
}



void GameMapWidget::paintEvent(QPaintEvent* event) {
    /*if (isUpdating) {
        qDebug() << "Repaint skipped - updating bullets!";
        return;
    }*/
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    int rows = mapData.size();
    int cols = mapData[0].size();
    float cellWidth = width() / float(cols);
    float cellHeight = height() / float(rows); 
    float cellSize = qMin(cellWidth, cellHeight); 
    float offsetX = (width() - cellSize * cols) / 2.0f;  
    float offsetY = (height() - cellSize * rows) / 2.0f; 

    int playerIndex = 0;

    for (int y = 0; y < rows; ++y) {
        for (int x = 0; x < cols; ++x) {
            QRectF cellRect(offsetX + x * cellSize, offsetY + y * cellSize, cellSize, cellSize);
            switch (mapData[y][x]) {
            case 0:
            {
                painter.fillRect(cellRect, QColor("#d3d3d3"));
                //painter.drawPixmap(cellRect.toRect(), playerTextures[playerIndex].scaled(cellSize, cellSize, Qt::KeepAspectRatio));
                //transform.reset();
                QPixmap rotatedPlayer = playerTextures[playerIndex].transformed(transform);
                painter.drawPixmap(cellRect.toRect(), rotatedPlayer.scaled(cellSize, cellSize, Qt::KeepAspectRatio));
                playerIndex = (playerIndex + 1) % 4;

                break;
            }
            case 1:
              painter.fillRect(cellRect, QColor("#d3d3d3"));
              break;
              
            case 2:
              painter.fillRect(cellRect, QColor("#008000"));
              painter.drawPixmap(cellRect.toRect(), wallTexture /*.scaled(cellSize, cellSize, Qt::IgnoreAspectRatio)*/);
              
              break;
            case 3:
              painter.fillRect(cellRect, QColor("#ff0000"));
              painter.drawPixmap(cellRect.toRect(), bombTexture.scaled(cellSize, cellSize, Qt::KeepAspectRatio));
             
              break;
            case 4:
              painter.fillRect(cellRect, QColor("#0000ff"));
              painter.drawPixmap(cellRect.toRect(), wallTexture.scaled(cellSize, cellSize, Qt::KeepAspectRatio));
              
              break;
            }
        }
    }

    QSharedPointer<QVector<BulletInfo>> localBullets;
    {
        QMutexLocker lock(&bulletsMutex);
        localBullets = bullets;
    }

    if (!bullets) {  
        qDebug() << "Bullets pointer is null! Skipping paintEvent.";
        return;
    }

    if (bullets->isEmpty()) {
        qDebug() << "No bullets to draw.";
        return;
    }
    qDebug() << "Current bullets:";

    for (const auto& bullet : *bullets) {
        float bulletSize = cellSize * 0.5; 
        float offsetXBullet = (cellSize - bulletSize) / 2.0;  
        float offsetYBullet = (cellSize - bulletSize) / 2.0;  

        QRectF bulletRect(
            offsetX + bullet.y * cellSize + offsetXBullet,
            offsetY + bullet.x * cellSize + offsetYBullet,
            bulletSize,
            bulletSize
        );

        /*painter.setBrush(Qt::yellow);  
        painter.setPen(Qt::NoPen);    
        painter.drawEllipse(bulletRect);  */
        
        painter.drawPixmap(bulletRect.toRect(), bulletTexture.scaled(cellSize, cellSize, Qt::KeepAspectRatio));  
    }

    
}

void GameMapWidget::keyPressEvent(QKeyEvent* event) {
    static QString currentDirection;

    switch (event->key()) {
    case Qt::Key_W:
    {
        currentDirection = "w";
        qDebug() << "Move Up";
        transform.reset();
        transform.rotate(270);
       // httpClient->movePlayer(sessionId, username, currentDirection);
        movePlayer("w");
        break;
    }
    case Qt::Key_S:
    {
        currentDirection = "s";
        qDebug() << "Move Down";
        transform.reset();
        transform.rotate(90);
        //httpClient->movePlayer(sessionId, username, currentDirection);
        movePlayer("s");
        break;
    }
    case Qt::Key_A:
    {
        currentDirection = "a";
        qDebug() << "Move Left";
        transform.reset();
        transform.scale(-1, 1);
       // httpClient->movePlayer(sessionId, username, currentDirection);
        movePlayer("a");
        break;
    }
    case Qt::Key_D:
    {
        currentDirection = "d";
        qDebug() << "Move Right";
        transform.reset();
        transform.rotate(0);
       // httpClient->movePlayer(sessionId, username, currentDirection);
        movePlayer("d");
        break;
    }
    case Qt::Key_Space:
        if (!currentDirection.isEmpty() && !isUpdating) {
            shootBullet(currentDirection);
        }
        break;

    default:
        QMainWindow::keyPressEvent(event);
    }
}


void GameMapWidget::shootBullet(const QString& direction) {
    if (sessionId.isEmpty() || username.isEmpty()) {
        qDebug() << "Error: sessionId or username is empty!";
        return;
    }

   
    QJsonObject requestData;
    requestData["sessionId"] = sessionId;
    requestData["username"] = username;
    requestData["direction"] = direction;

    QNetworkRequest request(QUrl("http://localhost:8080/game/shoot"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    
    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    QNetworkReply* reply = manager->post(request, QJsonDocument(requestData).toJson());

    connect(reply, &QNetworkReply::finished, [this, reply, direction]() {
        QByteArray responseData = reply->readAll();
        qDebug() << "Server response for shootBullet:" << responseData;

        
        auto jsonResponse = QJsonDocument::fromJson(responseData).object();
        if (jsonResponse.contains("startX") && jsonResponse.contains("startY")) {
            int startX = jsonResponse["startX"].toInt();
            int startY = jsonResponse["startY"].toInt();

            
            QMutexLocker lock(&bulletsMutex);
           // bullets->append(BulletInfo(startX, startY, direction[0].toLatin1()));
            qDebug() << "Added bullet locally at: (" << startY << ", " << startX << ")";
            update();
        }
        else {
            qDebug() << "Invalid response from server for shootBullet!";
        }

        //update();  
        reply->deleteLater();
        });
}



void GameMapWidget::syncBullets(const QString& sessionId) {
    if (sessionId.isEmpty()) {
        qDebug() << "Error: Session ID is empty!";
        return;
    }

  
    QJsonObject requestData;
    requestData["sessionId"] = sessionId;
    qDebug() << "Session id: "<<sessionId<<"\n";

    QNetworkRequest request(QUrl("http://localhost:8080/game/syncBullets"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

   
    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    QNetworkReply* reply = manager->post(request, QJsonDocument(requestData).toJson());

    connect(reply, &QNetworkReply::finished, [this, reply]() {
        QByteArray responseData = reply->readAll();
        auto jsonResponse = QJsonDocument::fromJson(responseData).object();

        if (jsonResponse.contains("bullets")) {
            QJsonArray bulletsArray = jsonResponse["bullets"].toArray();

           
            QMutexLocker lock(&bulletsMutex);
            bullets->clear();

            for (const auto& bulletValue : bulletsArray) {
                QJsonObject bulletObj = bulletValue.toObject();
                int x = bulletObj["x"].toInt();
                int y = bulletObj["y"].toInt();
                char direction = bulletObj["direction"].toString().toLatin1()[0];

                bullets->append(BulletInfo(x, y, direction));
                qDebug() << "Bullet synced: (" << x << ", " << y << "), Direction:" << direction;
            }

            
            update();
        }
        else {
            qDebug() << "Error: Server response does not contain 'bullets'";
        }
        //update();
        reply->deleteLater();
        });
}


void GameMapWidget::updateMapCells(const QVector<QPair<int, int>>& updatedCells) {
    QMutexLocker lock(&mapMutex);  // Protejează accesul la `mapData` dacă este accesat din thread-uri diferite

    for (const auto& cell : updatedCells) {
        int x = cell.first;
        int y = cell.second;

        if (x >= 0 && x < mapData.size() && y >= 0 && y < mapData[0].size()) {
            mapData[x][y] = 1;  // Setează celula ca spațiu liber (valoarea 1)
        }
        else {
            qDebug() << "Invalid map coordinates: (" << x << ", " << y << ")";
        }
    }
    update();
     // Declanșează redesenarea interfeței
}


void GameMapWidget::updateWalls() {
    if (sessionId.isEmpty()) {
        qDebug() << "Error: Session ID is empty!";
        return;
    }

    QJsonObject requestData;
    requestData["sessionId"] = sessionId;

    QNetworkRequest request(QUrl("http://localhost:8080/game/updateWalls"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    QNetworkReply* reply = manager->post(request, QJsonDocument(requestData).toJson());

    connect(reply, &QNetworkReply::finished, [this, reply]() {
        QByteArray responseData = reply->readAll();
        auto jsonResponse = QJsonDocument::fromJson(responseData).object();
        qDebug() << "Received response:" << responseData;
        if (responseData.isEmpty()) return;
        if (jsonResponse.contains("updatedCells")) {
            QJsonArray updatesArray = jsonResponse["updatedCells"].toArray();
            
           // updatedCells.clear();
            for (const auto& cellValue : updatesArray) {
                QJsonObject cellObj = cellValue.toObject();
                int x = cellObj["x"].toInt();
                int y = cellObj["y"].toInt();

                //updatedCells.append(qMakePair(x, y));
                mapData[x][y] = 1;

            }
            update();

           // updateMapCells(updatedCells);  // Actualizează harta

        }
        else {
            qDebug() << "Error: No 'updatedCells' in server response.";
        }
        
        reply->deleteLater();
        });
}

//void GameMapWidget::notifyServerBombTriggered(int bombX, int bombY) {
//    QJsonObject requestData;
//    requestData["sessionId"] = sessionId; // Add the session ID
//    requestData["bombX"] = bombX;         // Add the X coordinate of the bomb
//    requestData["bombY"] = bombY;         // Add the Y coordinate of the bomb
//
//    QNetworkRequest request(QUrl("http://localhost:8080/game/triggerBomb"));
//    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
//
//    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
//    QNetworkReply* reply = manager->post(request, QJsonDocument(requestData).toJson());
//
//    connect(reply, &QNetworkReply::finished, [this, reply]() {
//        QByteArray responseData = reply->readAll();
//        auto jsonResponse = QJsonDocument::fromJson(responseData).object();
//
//        if (jsonResponse.contains("updatedCells")) {
//            QJsonArray updatedCellsArray = jsonResponse["updatedCells"].toArray();
//
//            QVector<QPair<int, int>> updatedCells;
//            for (const auto& cellValue : updatedCellsArray) {
//                QJsonObject cellObj = cellValue.toObject();
//                int x = cellObj["x"].toInt();
//                int y = cellObj["y"].toInt();
//                updatedCells.append(qMakePair(x, y));
//            }
//
//            updateMapCells(updatedCells); // Update map with the affected cells
//        }
//        else {
//            qDebug() << "Error: No 'updatedCells' in server response.";
//        }
//
//        reply->deleteLater();
//        });
//}





void GameMapWidget::syncPlayers() {
    qDebug() << "[DEBUG] syncPlayers called for session:" << sessionId;

    if (sessionId.isEmpty()) {
        qDebug() << "[ERROR] Session ID is empty!";
        return;
    }

    QJsonObject requestData;
    requestData["sessionId"] = sessionId;

    QNetworkRequest request(QUrl("http://localhost:8080/game/syncPlayers"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    QNetworkReply* reply = manager->post(request, QJsonDocument(requestData).toJson());

    connect(reply, &QNetworkReply::finished, [this, reply]() {
        QByteArray responseData = reply->readAll();
        qDebug() << "[DEBUG] Response data from syncPlayers:" << responseData;

        QJsonObject jsonResponse = QJsonDocument::fromJson(responseData).object();

        if (jsonResponse.contains("players")) {

            //for (int row = 0; row < mapData.size(); ++row) {
            //    for (int col = 0; col < mapData[row].size(); ++col) {
            //        if (mapData[row][col] == 0) {  // Presupunând că 2 reprezintă jucători
            //            mapData[row][col] = 1;    // Setăm celula ca liberă
            //        }
            //    }
            //}
            QJsonArray playersArray = jsonResponse["players"].toArray();
            qDebug() << "[DEBUG] Number of players in response:" << playersArray.size();

            for (const auto& playerValue : playersArray) {
                QJsonObject playerObj = playerValue.toObject();
                qDebug() << "[DEBUG] Player object:" << playerObj;

                QString username = playerObj["username"].toString();
                int x = playerObj["x"].toInt();
                int y = playerObj["y"].toInt();

               // mapData[x][y] = 0;

                playerPositions[username] = QPoint(x, y);
                qDebug() << "[DEBUG] Updated player position for " << username
                    << ": (" << x << ", " << y << ")";
            }
            update();  // Re-desenăm harta
        }
        else {
            qDebug() << "[ERROR] No 'players' data in server response.";
        }
        reply->deleteLater();
        });

}



void GameMapWidget::displayMap()
{
    qDebug() << "[DEBUG] Player positions:";
    for (auto it = playerPositions.constBegin(); it != playerPositions.constEnd(); ++it) {
        QString username = it.key();  
        QPoint position = it.value(); 

        qDebug() << "Player:" << username
            << "Position: (" << position.x() << ", " << position.y() << ")";
    }
    qDebug() << "[DEBUG] Current map state:";
    for (int row = 0; row < mapData.size(); ++row) {
        QString rowString;
        for (int col = 0; col < mapData[row].size(); ++col) {
            rowString += QString::number(mapData[row][col]) + " ";
        }
        qDebug() << rowString;
    }
}

//void GameMapWidget::movePlayer(const QString& direction) {
//    // Obține poziția curentă a jucătorului
//    QPoint currentPosition = playerPositions[username];
//    QPoint newPosition = currentPosition;
//    mapData[currentPosition.x()][currentPosition.y()] = 1;
//    // Actualizează poziția pe baza direcției
//    if (direction == "w") {
//        newPosition.setY(currentPosition.y() - 1);  // Deplasare în sus
//    }
//    else if (direction == "s") {
//        newPosition.setY(currentPosition.y() + 1);  // Deplasare în jos
//    }
//    else if (direction == "a") {
//        newPosition.setX(currentPosition.x() - 1);  // Deplasare la stânga
//    }
//    else if (direction == "d") {
//        newPosition.setX(currentPosition.x() + 1);  // Deplasare la dreapta
//    }
//
//    // Actualizează poziția locală
//    playerPositions[username] = newPosition;
//    qDebug() << "[DEBUG] Player moved locally:" << username << "to position:" << newPosition;
//   // mapData[newPosition.x()][newPosition.y()] = 0;
//
//    // Trimite cererea către server pentru sincronizare
//    QJsonObject requestData;
//    requestData["sessionId"] = sessionId;
//    requestData["username"] = username;
//    requestData["direction"] = direction;
//
//    QNetworkRequest request(QUrl("http://localhost:8080/game/move"));
//    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
//
//    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
//    QNetworkReply* reply = manager->post(request, QJsonDocument(requestData).toJson());
//
//    connect(reply, &QNetworkReply::finished, [this, reply]() {
//        QByteArray responseData = reply->readAll();
//        QJsonObject jsonResponse = QJsonDocument::fromJson(responseData).object();
//        qDebug() << "[DEBUG] Server response for movePlayer:" << jsonResponse;
//
//        // Actualizează pozițiile celorlalți jucători
//        if (jsonResponse.contains("players")) {
//            QJsonArray playersArray = jsonResponse["players"].toArray();
//            for (const auto& playerValue : playersArray) {
//                QJsonObject playerObj = playerValue.toObject();
//                QString username = playerObj["username"].toString();
//                int x = playerObj["x"].toInt();
//                int y = playerObj["y"].toInt();
//                playerPositions[username] = QPoint(x, y);
//                mapData[x][y] = 0;
//                
//            }
//            update();  // Re-desenăm harta
//        }
//
//        reply->deleteLater();
//        });
//
//    update();  // Re-desenăm harta imediat
//}


void GameMapWidget::movePlayer(const QString& direction) {
    // Trimite cererea către server pentru sincronizare
    QJsonObject requestData;
    requestData["sessionId"] = sessionId;
    requestData["username"] = username;
    requestData["direction"] = direction;

    QNetworkRequest request(QUrl("http://localhost:8080/game/move"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    QNetworkReply* reply = manager->post(request, QJsonDocument(requestData).toJson());

    connect(reply, &QNetworkReply::finished, [this, reply]() {
        QByteArray responseData = reply->readAll();
        QJsonObject jsonResponse = QJsonDocument::fromJson(responseData).object();
        qDebug() << "[DEBUG] Server response for movePlayer:" << jsonResponse;

        // Procesăm pozițiile jucătorilor returnate de server
        if (jsonResponse.contains("players")) {
            QJsonArray playersArray = jsonResponse["players"].toArray();

            // Curățăm harta de pozițiile vechi
            for (int row = 0; row < mapData.size(); ++row) {
                for (int col = 0; col < mapData[row].size(); ++col) {
                    if (mapData[row][col] == 0) {  // Presupunând că 2 reprezintă un jucător
                        mapData[row][col] = 1;    // Setăm celula ca liberă
                    }
                }
            }

            // Actualizăm pozițiile primite de la server
            for (const auto& playerValue : playersArray) {
                QJsonObject playerObj = playerValue.toObject();
                QString username = playerObj["username"].toString();
                int x = playerObj["x"].toInt();
                int y = playerObj["y"].toInt();

                playerPositions[username] = QPoint(x, y);  // Actualizăm poziția locală
                mapData[x][y] = 0;  // Actualizăm harta
                qDebug() << "[DEBUG] Updated player position for " << username << ": (" << x << ", " << y << ")";
            }
            update();  // Re-desenăm harta
        }
        else {
            qDebug() << "[ERROR] No 'players' data in server response.";
        }

        reply->deleteLater();
        });
}


