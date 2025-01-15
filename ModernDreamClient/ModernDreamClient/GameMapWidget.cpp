//#include "GameMapWidget.h"
//#include <QPainter>
//
//GameMapWidget::GameMapWidget(QWidget* parent) : QWidget(parent) {
//    loadTextures();
//    setFixedSize(1200, 800);
//}
//
//void GameMapWidget::loadTextures() {
//    wallTexture.load("../ModernDreamImages/tire1.png");
//    wallTexture = wallTexture.scaled(CELL_SIZE, CELL_SIZE, Qt::KeepAspectRatio);
//
//    bombTexture.load("../ModernDreamImages/tire1.png");
//    bombTexture = bombTexture.scaled(CELL_SIZE, CELL_SIZE, Qt::KeepAspectRatio);
//
//    playerTextures.resize(4);
//    playerTextures[0].load("../ModernDreamImages/carBlueRight.png");
//    playerTextures[1].load("../ModernDreamImages/carGreenRight.png");
//    playerTextures[2].load("../ModernDreamImages/carPinkRight.png");
//    playerTextures[3].load("../ModernDreamImages/carYellowRight.png");
//
//    for (auto& texture : playerTextures) {
//        texture = texture.scaled(CELL_SIZE, CELL_SIZE, Qt::KeepAspectRatio);
//    }
//}
//
//void GameMapWidget::initializeMap(const QVector<QVector<int>>& mapData) {
//    map = mapData;
//    update();
//}
//
//void GameMapWidget::paintEvent(QPaintEvent* event) {
//    QPainter painter(this);
//    painter.setRenderHint(QPainter::Antialiasing);
//
//    for (int y = 0; y < map.size(); ++y) {
//        for (int x = 0; x < map[y].size(); ++x) {
//            QRect cellRect(x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE);
//
//            switch (map[y][x]) {
//            case 0: // Player position
//                painter.fillRect(cellRect, QColor("#ffffff"));
//                painter.drawPixmap(cellRect, playerTextures[0]); 
//                break;
//            case 1: // Free space
//                painter.fillRect(cellRect, QColor("#d3d3d3"));
//                break;
//            case 2: // Destructible wall
//                painter.fillRect(cellRect, QColor("#008000"));
//                painter.drawPixmap(cellRect, wallTexture);
//                break;
//            case 3: // Destructible wall with bomb
//                painter.fillRect(cellRect, QColor("#ff0000"));
//                painter.drawPixmap(cellRect, bombTexture);
//                break;
//            case 4: // Non-destructible wall
//                painter.fillRect(cellRect, QColor("#0000ff"));
//                painter.drawPixmap(cellRect, wallTexture);
//                break;
//            }
//        }
//    }
//}
#include "GameMapWidget.h"
#include <QPainter>
#include <QLabel>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include<QMutexLocker>
#include <vector>

GameMapWidget::GameMapWidget(QWidget* parent) : QWidget(parent), httpClient(new HttpClient(this)){
    loadTextures();
    setMinimumSize(1400, 800);  

    //QTimer* bulletSyncTimer = new QTimer(this);  // Creează un timer
    //connect(bulletSyncTimer, &QTimer::timeout, this, &GameMapWidget::syncBulletsFromServer);  // Conectează timeout-ul la funcția de sincronizare
    //bulletSyncTimer->start(1000);
    //
}

void GameMapWidget::loadTextures() {
    wallTexture.load("../ModernDreamImages/tire2.png");
    //wallTexture = wallTexture.scaled(CELL_SIZE, CELL_SIZE, Qt::IgnoreAspectRatio);

    bombTexture.load("../ModernDreamImages/tire2.png");
    //bombTexture = bombTexture.scaled(CELL_SIZE, CELL_SIZE, Qt::IgnoreAspectRatio);

    bulletTexture.load("../ModernDreamImages/bullet2.png"); 
    if (bulletTexture.isNull()) {
        qDebug() << "Failed to load bullet texture from file!";
    }
    

    playerTextures.resize(4);
    playerTextures[0].load("../ModernDreamImages/carBlueRight.png");
    playerTextures[1].load("../ModernDreamImages/carGreenRight.png");
    playerTextures[2].load("../ModernDreamImages/carPinkRight.png");
    playerTextures[3].load("../ModernDreamImages/carYellowRight.png");

    /*for (auto& texture : playerTextures) {
        texture = texture.scaled(CELL_SIZE, CELL_SIZE, Qt::IgnoreAspectRatio);
    }*/
    

}

void GameMapWidget::initializeMap(const QVector<QVector<int>>& mapData) {
    map = mapData;
    //qDebug() << "Client map initialized with size:" << map.size() << "rows x" << (map.isEmpty() ? 0 : map[0].size()) << "cols";
    update();
    //qDebug() << "Harta reinițializată. Poziția jucătorului: (" << playerX << ", " << playerY << ")";

}



void GameMapWidget::paintEvent(QPaintEvent* event) {
    // qDebug() << "paintEvent apelat pentru poziția jucătorului: (" << playerX << ", " << playerY << ")";
     //qDebug() << "Skipping recursive paintEvent!";
    if (isPainting) {
        qDebug() << "Repaint already in progress!";
        return;
    }
    isPainting = true;

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);


    if (map.isEmpty() || map[0].isEmpty()) {
        return;
    }

    int rows = map.size();
    int cols = map[0].size();

    // Dimensiunea fiecărei celule
    float cellWidth = width() / float(cols);
    float cellHeight = height() / float(rows);
    float cellSize = qMin(cellWidth, cellHeight);

    float offsetX = (width() - cellSize * cols) / 2.0f;
    float offsetY = (height() - cellSize * rows) / 2.0f;

    QPen pen(Qt::gray);
    pen.setWidth(0.2);
    painter.setPen(pen);

    int playerIndex = 0;

    qDebug() << "Intra in for\n";
    for (int y = 0; y < rows; ++y) {
        for (int x = 0; x < cols; ++x) {
            QRectF cellRect(offsetX + x * cellSize, offsetY + y * cellSize, cellSize, cellSize);
            
            switch (map[y][x]) {
            case 0: // Jucător
                /*painter.fillRect(cellRect, QColor("#d3d3d3"));
                painter.drawPixmap(cellRect.toRect(), playerTextures[0].scaled(cellSize, cellSize, Qt::KeepAspectRatio));
                break;*/
                painter.fillRect(cellRect, QColor("#d3d3d3"));

                painter.drawPixmap(cellRect.toRect(), playerTextures[playerIndex].scaled(cellSize, cellSize, Qt::KeepAspectRatio));

                playerIndex = (playerIndex + 1) % 4;
                qDebug() << "Desen jucator\n";
                break;

            case 1:
                painter.fillRect(cellRect, QColor("#d3d3d3"));
                break;
                qDebug() << "Perete 1\n";
            case 2:
                painter.fillRect(cellRect, QColor("#008000"));
                painter.drawPixmap(cellRect.toRect(), wallTexture.scaled(cellSize, cellSize, Qt::KeepAspectRatio));
                qDebug() << "Perete 2\n";
                break;
            case 3:
                painter.fillRect(cellRect, QColor("#ff0000"));
                painter.drawPixmap(cellRect.toRect(), bombTexture.scaled(cellSize, cellSize, Qt::KeepAspectRatio));
                qDebug() << "Perete 3\n";
                break;
            case 4:
                painter.fillRect(cellRect, QColor("#0000ff"));
                painter.drawPixmap(cellRect.toRect(), wallTexture.scaled(cellSize, cellSize, Qt::KeepAspectRatio));
                qDebug() << "Perete 4\n";
                break;
            }

            painter.drawRect(cellRect);
        }
    }

    

    //QMutexLocker lock(&bulletsMutex);  // Ensure thread-safety for bullets

    //for (const auto& bullet : bullets) {
    //    if (bullet.x < 0 || bullet.y < 0 || bulletTexture.isNull()) {
    //        qDebug() << "Skipping invalid bullet at (" << bullet.x << ", " << bullet.y << ")";
    //        continue;  // Skip invalid bullets
    //    }
    //    else qDebug() << "Drawing buulet at coordinates " << bullet.x << " " << bullet.y;

    //   /* QRect bulletRect(bullet.x * CELL_SIZE, bullet.y * CELL_SIZE, CELL_SIZE, CELL_SIZE);
    //    painter.drawPixmap(bulletRect, bulletTexture.scaled(CELL_SIZE, CELL_SIZE, Qt::KeepAspectRatio));*/

    //    QRect bulletRect(offsetX+bullet.x * cellSize, offsetY+bullet.y * cellSize, cellSize, cellSize);
    //    painter.setBrush(Qt::yellow);
    //    painter.drawEllipse(bulletRect);  // Desenează glonțul ca un cerc simplu pentru testare
    
        //QMutexLocker lock(&bulletsMutex);
        //auto localBulletPositions = bulletPositions; // Copy for thread safety
        //auto localBulletDirections = bulletDirections;
        //lock.unlock();


        //for (int i = 0; i < localBulletPositions.size(); ++i) {
        //    const auto& pos = localBulletPositions[i];
        //    if (pos.first < 0 || pos.second < 0) {
        //        continue; // Skip invalid bullets
        //    }
        //    QRect bulletRect(offsetX + pos.first * cellSize, offsetY + pos.second * cellSize, cellSize, cellSize);
        //    painter.setBrush(Qt::yellow);
        //    painter.drawEllipse(bulletRect); // Draw bullet as a circle

        //}
        // 
        // 
        // Paint bullets and check collisions
        //QMutexLocker lock(&bulletsMutex);
        //for (auto it = bulletPositions.begin(); it != bulletPositions.end();) {
        //    const auto& bullet = *it;
        //    QRectF bulletRect(offsetX + bullet.first * cellSize, offsetY + bullet.second * cellSize, cellSize, cellSize);

        //    bool collision = false;

        //    // Check collision with walls
        //    if (map[bullet.second][bullet.first] == 1 || map[bullet.second][bullet.first] == 3) {
        //        collision = true;
        //    }

            // Check collision with players
            /*for (const auto& playerPos : playerPositions) {
                if (bullet.first == playerPos.first && bullet.second == playerPos.second) {
                    collision = true;
                    emit playerHit(playerPos);
                    break;
                }
            }*/

            //if (collision) {
            //    it = bulletPositions.erase(it);
            //}
            //else {
            //    painter.setBrush(Qt::yellow);
            //    painter.drawEllipse(bulletRect);
            //    ++it;
            //}
            painter.drawEllipse(50, 50, 20 + test, 20);
            test += 1;
            //for (auto it = bulletPositions.begin(); it != bulletPositions.end();) {
            //  const auto& bullet = *it;
            //
            //  if (bullet.first >= 0 && bullet.second >= 0) {  // Valid position
            //      QRectF bulletRect(offsetX + bullet.first * cellSize, offsetY + bullet.second * cellSize, cellSize, cellSize);

            //      //bool collision = (map[bullet.second][bullet.first] == 2 || map[bullet.second][bullet.first] == 3);
            //      //if (collision) {
            //      //    qDebug() << "Bullet collided at (" << bullet.first << "," << bullet.second << ")";
            //      //    it = bulletPositions.erase(it);  // Remove collided bullet
            //      //}
            //      //else {
            //      painter.setBrush(Qt::yellow);
            //      painter.drawEllipse(bulletRect);  // Draw bullet
            //      ++it;

            //      //}
            //  }
            //  else {
            //    ++it;  // Skip invalid bullets
            //  }

            //}

           
            for (const auto& bullet : bulletPositions) {
                if (bulletPositions.isEmpty()) {
                    qDebug() << "No bullets to draw!";
                    return;  // Nu desenează dacă lista este goală
                }
                qDebug() << "Processing bullet at position:" << bullet;
                if (bullet.first < 0 || bullet.second < 0 || bullet.first >= cols || bullet.second >= rows) {
                    qDebug() << "Skipping invalid bullet at position:" << bullet;
                    continue;
                }

                QRectF bulletRect(offsetX + bullet.first * cellSize, offsetY + bullet.second * cellSize, cellSize, cellSize);
                qDebug() << "Bullet rect:" << bulletRect;
                painter.drawEllipse(bulletRect);  // Draw the bullet
            }

            //{
            //    QMutexLocker lock(&bufferMutex);
            //    bulletPositions = bulletBuffer;  // Actualizează lista doar din buffer-ul temporar
            //}

            //for (const QPair<int, int>& bullet : bulletPositions) {
            //    int x = bullet.first;
            //    int y = bullet.second;

            //    QRect bulletRect(offsetX + x * cellSize, offsetY + y * cellSize, cellSize, cellSize);
            //    painter.setBrush(Qt::yellow);
            //    painter.drawEllipse(bulletRect);  // Desenează un cerc galben la poziția (x, y)
            //}

        isPainting = false;

    

}






void GameMapWidget::updatePlayerPosition(int x, int y) {
    qDebug() << "Actualizare poziție jucător la: (" << x << ", " << y << ")";
    for (int i = 0; i < map.size(); ++i) {
        for (int j = 0; j < map[i].size(); ++j) {
            if (map[i][j] == 0) {  // Poziția veche a jucătorului
                map[i][j] = 1;  // Setăm spațiu liber
            }
        }
    }

    map[x][y] = 0;  // Setăm noua poziție a jucătorului
   // playerX = x;  // Actualizează poziția curentă a jucătorului
   // playerY = y;
    //repaint(); // Redesenăm harta
    //animatePlayerMove(playerX, playerY, x, y);
    update();
}

//void GameMapWidget::updateBullets(const QVector<BulletInfo>& newBullets) {
//    QMutexLocker lock(&bulletsMutex);  // Ensure thread-safety
//    bullets = newBullets;
//    for (const auto& bullet : bullets) {
//        qDebug() << "Updated bullet at position (" << bullet.x << ", " << bullet.y << ")";
//    }
//    update();  // Trigger repaint
//}


//void GameMapWidget::updateBullets(const QVector<BulletInfo>& newBullets) {
//    QMetaObject::invokeMethod(this, [newBullets, this]() {
//        QMutexLocker lock(&bulletsMutex);  // Blochează accesul
//        bullets.detach();  // Evită accesul partajat
//        bullets = newBullets;  // Actualizare
//        qDebug() << "Updated bullets: " << bullets.size();
//        update();  // Actualizare interfață
//        }, Qt::QueuedConnection);
//}


//void GameMapWidget::updateBullets(const QVector<BulletInfo>& newBullets) {
//    if (newBullets.empty()) {
//        qDebug() << "Received empty bullet list!";
//        return;
//    }
//    qDebug() << "Number of bullets received: " << newBullets.size();
//    for (const auto& bullet : newBullets) {
//        qDebug() << "Bullet coordinates: " << bullet.x << ", " << bullet.y;
//    }
//
//    QMutexLocker lock(&bulletsMutex);
//    //bullets = newBullets;  
//    bullets = newBullets;  // Mută datele în loc să le copiezi
//
//    update();  
//}




//void GameMapWidget::updateBullets(const QVector<BulletInfo>& newBullets) {
//    if (newBullets.empty()) {
//        qDebug() << "Received empty bullet list!";
//        return;
//    }
//
//    /*if (bullets == newBullets) {
//        qDebug() << "Skipping duplicate bullet update.";
//        return; 
//    }*/
//
//    qDebug() << "Number of bullets received: " << newBullets.size();
//    for (const auto& bullet : newBullets) {
//        qDebug() << "Bullet coordinates: " << bullet.x << ", " << bullet.y;
//    }
//
//    QMutexLocker lock(&bulletsMutex);
//    bullets.clear();
//    for (const auto& bullet : newBullets) {
//        if (bullet.x >= 0 && bullet.y >= 0 && bulletTexture.isNull() == false) {
//            bullets.push_back(bullet);
//        }
//    }
//    update();
//}


//void GameMapWidget::updateBullets(const QVector<QPair<int, int>>& newPositions, const QVector<char>& newDirections) {
//    if (newPositions.isEmpty() || newDirections.isEmpty()) {
//        qDebug() << "Received empty bullet data!";
//        return;
//    }
//
//    if (newPositions.size() != newDirections.size()) {
//        qDebug() << "Mismatch between bullet positions and directions!";
//        return;
//    }
//
//    qDebug() << "Number of bullets received: " << newPositions.size();
//    for (int i = 0; i < newPositions.size(); ++i) {
//        qDebug() << "Bullet coordinates: " << newPositions[i].first << ", " << newPositions[i].second
//            << " Direction: " << newDirections[i];
//    }
//
//    // Update the bullets (thread-safe)
//    QMutexLocker lock(&bulletsMutex);
//    bulletPositions = newPositions;
//    bulletDirections = newDirections;
//
//    // Trigger a repaint
//    update();
//}

//void GameMapWidget::updateBullets(const QVector<QPair<int, int>>& newPositions, const QVector<char>& newDirections) {
//    QMutexLocker lock(&bulletsMutex);
//
//    if (newPositions.isEmpty() || newDirections.isEmpty()) {
//        qDebug() << "Received empty bullet data!";
//        return;
//    }
//
//    if (newPositions == bulletPositions && newDirections == bulletDirections) {
//        qDebug() << "Skipping duplicate update.";
//        return;
//    }
//
//    // Debug new data
//    qDebug() << "New bullet positions:" << newPositions;
//    qDebug() << "New bullet directions:" << newDirections;
//
//    // Update internal data safely
//    bulletPositions = newPositions;
//    bulletDirections = newDirections;
//
//    lock.unlock(); // Release mutex
//
//    // Trigger a repaint
//    update();
//}

//void GameMapWidget::updateBullets(const QVector<QPair<int, int>>& newPositions, const QVector<char>& newDirections) {
//    {
//        QMutexLocker lock(&bulletsMutex);
//
//        if (newPositions == bulletPositions && newDirections == bulletDirections) {
//            qDebug() << "Skipping duplicate update.";
//            return;
//        }
//
//        bulletPositions = newPositions;
//        bulletDirections = newDirections;
//    } // Mutex released here
//
//    // Defer repaint
//    QMetaObject::invokeMethod(this, "update", Qt::QueuedConnection);
//}

void GameMapWidget::updateBullets(const QVector<QPair<int, int>>& newBulletPositions) {
    //QMutexLocker lock(&bulletsMutex);
    bulletPositions = newBulletPositions;
    //update();
}



void GameMapWidget::setSessionId(const QString& sessionId) {
    currentSessionId = sessionId;
    qDebug() << "Session ID set in GameMapWidget:" << currentSessionId;
}


//void GameMapWidget::syncBulletsFromServer() {
//    if (syncInProgress) {
//        qDebug() << "Sync in progress, skipping new sync request.";
//        return;  // Evită trimiterea unei cereri noi dacă deja se sincronizează
//    }
//    syncInProgress = true;
//    QNetworkRequest request(QUrl("http://localhost:8080/game/syncBullets"));
//    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
//
//    QJsonObject data;
//    data["sessionId"] = currentSessionId;
//    QNetworkReply* reply = httpClient->manager->post(request, QJsonDocument(data).toJson());
//
//    connect(reply, &QNetworkReply::finished, [this, reply]() {
//        syncInProgress = false;
//        QByteArray responseData = reply->readAll();
//        QJsonObject jsonResponse = QJsonDocument::fromJson(responseData).object();
//        QJsonArray bulletsArray = jsonResponse["bullets"].toArray();
//
//        QVector<QPair<int, int>> newBulletBuffer;
//        for (const QJsonValue& bulletValue : bulletsArray) {
//            QJsonObject bulletObj = bulletValue.toObject();
//            int x = bulletObj["x"].toInt();
//            int y = bulletObj["y"].toInt();
//            newBulletBuffer.push_back(qMakePair(x, y));
//        }
//
//        {
//            QMutexLocker lock(&bufferMutex);  // Protejează accesul la buffer
//            bulletBuffer = std::move(newBulletBuffer);
//        }
//       // update();
//        reply->deleteLater();
//       // update();  // Redesenare
//        });
//}
