#include "GameMapWidget.h"
#include <QPainter>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <QtGlobal> 

GameMapWidget::GameMapWidget(const QString& sessionId, const QString& username, QWidget* parent)
    : QMainWindow(parent), sessionId(sessionId), username(username) {
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
    connect(httpClient, &HttpClient::playerMoved, this, &GameMapWidget::updatePlayerPosition);

    QTimer* bulletSyncTimer = new QTimer(this); 
    connect(bulletSyncTimer, &QTimer::timeout, this, [this]() {   
        if (!isUpdating)
        {
            syncBullets(sessionId);
            //fetchAndInitializeMap();
            //syncMap();
            updateWalls();
        }
           
           
        });
    bulletSyncTimer->start(1000);  
}

void GameMapWidget::fetchAndInitializeMap() {
   
    QByteArray response = httpClient->requestMapGeneration(sessionId,1);
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
    if (isUpdating) {
        qDebug() << "Repaint skipped - updating bullets!";
        return;
    }
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
              painter.fillRect(cellRect, QColor("#d3d3d3"));
              painter.drawPixmap(cellRect.toRect(), playerTextures[playerIndex].scaled(cellSize, cellSize, Qt::KeepAspectRatio));
                
              playerIndex = (playerIndex + 1) % 4;
            
              break;
                
            case 1:
              painter.fillRect(cellRect, QColor("#d3d3d3"));
              break;
              
            case 2:
              painter.fillRect(cellRect, QColor("#008000"));
             // painter.drawPixmap(cellRect.toRect(), wallTexture.scaled(cellSize, cellSize, Qt::KeepAspectRatio));
              
              break;
            case 3:
              painter.fillRect(cellRect, QColor("#ff0000"));
             // painter.drawPixmap(cellRect.toRect(), bombTexture.scaled(cellSize, cellSize, Qt::KeepAspectRatio));
             
              break;
            case 4:
              painter.fillRect(cellRect, QColor("#0000ff"));
             // painter.drawPixmap(cellRect.toRect(), wallTexture.scaled(cellSize, cellSize, Qt::KeepAspectRatio));
              
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

        painter.setBrush(Qt::yellow);  
        painter.setPen(Qt::NoPen);    
        painter.drawEllipse(bulletRect);  
    }
}

void GameMapWidget::keyPressEvent(QKeyEvent* event) {
    static QString currentDirection;

    switch (event->key()) {
    case Qt::Key_W:
        currentDirection = "w";
        qDebug() << "Move Up";
        httpClient->movePlayer(sessionId, username, currentDirection);
        break;

    case Qt::Key_S:
        currentDirection = "s";
        qDebug() << "Move Down";
        httpClient->movePlayer(sessionId, username, currentDirection);
        break;

    case Qt::Key_A:
        currentDirection = "a";
        qDebug() << "Move Left";
        httpClient->movePlayer(sessionId, username, currentDirection);
        break;

    case Qt::Key_D:
        currentDirection = "d";
        qDebug() << "Move Right";
        httpClient->movePlayer(sessionId, username, currentDirection);
        break;

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
