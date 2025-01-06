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

GameMapWidget::GameMapWidget(QWidget* parent) : QWidget(parent) {
    loadTextures();
    setMinimumSize(1400, 800);  // Optional: Set a minimum size to avoid too small cells
}

void GameMapWidget::loadTextures() {
    wallTexture.load("../ModernDreamImages/tire2.png");
    wallTexture = wallTexture.scaled(CELL_SIZE, CELL_SIZE, Qt::IgnoreAspectRatio);

    bombTexture.load("../ModernDreamImages/tire2.png");
    bombTexture = bombTexture.scaled(CELL_SIZE, CELL_SIZE, Qt::IgnoreAspectRatio);

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
}

void GameMapWidget::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Calculate the cell size dynamically based on the widget size
    float cellWidth = width() / map[0].size()+0.9;
    float cellHeight = height() / map.size()+0.9;
    float cellSize = qMin(cellWidth, cellHeight); // Use the smaller of the two to keep cells square

    for (int y = 0; y < map.size(); ++y) {
        for (int x = 0; x < map[y].size(); ++x) {
            QRect cellRect(x * cellSize, y * cellSize, cellSize, cellSize);

            switch (map[y][x]) {
            case 0: // Player position
                painter.fillRect(cellRect, QColor("#ffffff"));
                painter.drawPixmap(cellRect, playerTextures[0]);
                break;
            case 1: // Free space
                painter.fillRect(cellRect, QColor("#878787"));
                break;
            case 2: // Destructible wall
                painter.fillRect(cellRect, QColor("#008000"));
                painter.drawPixmap(cellRect, wallTexture);
                break;
            case 3: // Destructible wall with bomb
                painter.fillRect(cellRect, QColor("#ff0000"));
                painter.drawPixmap(cellRect, bombTexture);
                break;
            case 4: // Non-destructible wall
                painter.fillRect(cellRect, QColor("#0000ff"));
                painter.drawPixmap(cellRect, wallTexture);
                break;
            }
        }
    }
}
