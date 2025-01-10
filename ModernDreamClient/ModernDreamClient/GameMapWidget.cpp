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


GameMapWidget::GameMapWidget(QWidget* parent) : QWidget(parent) {
    loadTextures();
    setMinimumSize(1400, 800);  
    
}

void GameMapWidget::loadTextures() {
    wallTexture.load("../ModernDreamImages/tire2.png");
    wallTexture = wallTexture.scaled(CELL_SIZE, CELL_SIZE, Qt::IgnoreAspectRatio);

    bombTexture.load("../ModernDreamImages/tire2.png");
    bombTexture = bombTexture.scaled(CELL_SIZE, CELL_SIZE, Qt::IgnoreAspectRatio);

    bulletTexture.load("../ModernDreamImages/bullet2.png");
    bulletTexture = bulletTexture.scaled(CELL_SIZE, CELL_SIZE, Qt::IgnoreAspectRatio);


    playerTextures.resize(4);
    playerTextures[0].load("../ModernDreamImages/carBlueRight.png");
    playerTextures[1].load("../ModernDreamImages/carGreenRight.png");
    playerTextures[2].load("../ModernDreamImages/carPinkRight.png");
    playerTextures[3].load("../ModernDreamImages/carYellowRight.png");

    for (auto& texture : playerTextures) {
        texture = texture.scaled(CELL_SIZE, CELL_SIZE, Qt::IgnoreAspectRatio);
    }
    

}

void GameMapWidget::initializeMap(const QVector<QVector<int>>& mapData) {
    map = mapData;
    update();
    //qDebug() << "Harta reinițializată. Poziția jucătorului: (" << playerX << ", " << playerY << ")";

}



void GameMapWidget::paintEvent(QPaintEvent* event) {
   // qDebug() << "paintEvent apelat pentru poziția jucătorului: (" << playerX << ", " << playerY << ")";
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
                break;
            
            case 1: 
                painter.fillRect(cellRect, QColor("#d3d3d3"));  
                break;
            case 2: 
                painter.fillRect(cellRect, QColor("#008000"));  
                painter.drawPixmap(cellRect.toRect(), wallTexture.scaled(cellSize, cellSize, Qt::KeepAspectRatio));
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

            painter.drawRect(cellRect);
        }
    }
    for (const auto& bullet : bullets) {
        QRectF bulletRect(offsetX + bullet.x * cellSize, offsetY + bullet.y * cellSize, cellSize, cellSize);
        painter.drawPixmap(bulletRect.toRect(), bulletTexture.scaled(cellSize, cellSize, Qt::IgnoreAspectRatio));
    }
    

     
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
    repaint();  // Redesenăm harta
    //animatePlayerMove(playerX, playerY, x, y);
}



//void GameMapWidget::updateBullets(const QVector<BulletInfo>& newBullets) {
//    bullets = newBullets;  
//    update();  
//}

void GameMapWidget::updateBullets(const QVector<BulletInfo>& newBullets) {
    qDebug() << "Updating bullets with positions:";
    for (const auto& bullet : newBullets) {
        qDebug() << "Bullet at (" << bullet.x << ", " << bullet.y << ")";
    }
    bullets = newBullets;  // Update the internal state
    update();  // Trigger a repaint
}

//void GameMapWidget::updateBullets(const QVector<BulletInfo>& bullets) {
//    if (bullets.isEmpty()) {
//        qDebug() << "No bullets to update.";
//        return;
//    }
//
//    QVector<BulletInfo> validBullets;
//    for (const auto& bullet : bullets) {
//        if (bullet.x >= 0 && bullet.x < mapWidth && bullet.y >= 0 && bullet.y < mapHeight) {
//            validBullets.push_back(bullet);
//        }
//        else {
//            qDebug() << "Invalid bullet position: (" << bullet.x << ", " << bullet.y << ")";
//        }
//    }
//
//    this->bullets = validBullets; // Only update with valid data
//    update(); // Trigger repaint
//}




