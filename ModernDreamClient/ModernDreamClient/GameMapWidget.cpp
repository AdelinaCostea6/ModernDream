#include "GameMapWidget.h"

GameMapWidget::GameMapWidget(QWidget* parent)
    : QWidget(parent), tileSize(40) /*,playerPosition(-1, -1)*/ {
    setAutoFillBackground(true);
    QPalette pal = palette();
    pal.setColor(QPalette::Window, QColor("#1F2937")); // Dark background
    setPalette(pal);

    tileImages[1] = QImage("../ModernDreamImages/tire1.png");  // Wall image
    

}

void GameMapWidget::setMapData(const QVector<QVector<int>>& data) {
    mapData = data;

    // Adjust widget size based on map dimensions
    if (!mapData.isEmpty()) {
        int width = mapData[0].size() * tileSize;
        int height = mapData.size() * tileSize;
        setMinimumSize(width, height);
        setMaximumSize(width, height);
    }

    update(); // Trigger repaint
}

void GameMapWidget::setTileSize(int size) {
    tileSize = size; 
    if (!mapData.isEmpty()) {
        int width = mapData[0].size() * tileSize;
        int height = mapData.size() * tileSize;
        setMinimumSize(width, height);
        setMaximumSize(width, height);
    }
    update();
}

void GameMapWidget::setTileImages(const QMap<int, QImage>& images) {
    tileImages = images;
}



//
//void GameMapWidget::paintEvent(QPaintEvent* /*event*/) {
//    QPainter painter(this);
//    painter.setRenderHint(QPainter::Antialiasing);
//
//    for (int i = 0; i < mapData.size(); ++i) {
//        for (int j = 0; j < mapData[i].size(); ++j) {
//            QRect tileRect(j * tileSize, i * tileSize, tileSize, tileSize);
//            drawTile(painter, mapData[i][j], tileRect);
//        }
//    }
//
//    // Draw player
//    //if (playerPosition.first != -1 && playerPosition.second != -1) {
//    //    QRect playerRect(playerPosition.second * tileSize, playerPosition.first * tileSize, tileSize, tileSize);
//    //    drawTile(painter, 3, playerRect); // Assuming 3 represents the player
//    //}
//}
void GameMapWidget::paintEvent(QPaintEvent* event) {
    QPainter painter(this);

    for (int row = 0; row < mapData.size(); ++row) {
        for (int col = 0; col < mapData[row].size(); ++col) {
            int x = col * tileSize;
            int y = row * tileSize;

            int tileType = mapData[row][col];

            // Draw the track (empty space) as a colored square
            if (tileType == 0) {
                painter.fillRect(x, y, tileSize, tileSize, trackColor);
            }
            // Draw the image corresponding to the tile type if available in tileImages map
            else if (tileImages.contains(tileType)) {
                painter.drawImage(x, y, tileImages[tileType].scaled(tileSize, tileSize));
            }
        }
    }
}


void GameMapWidget::drawTile(QPainter& painter, int tileType, const QRect& rect) {
    switch (tileType) {
    case 1: // Free space (track)
        painter.fillRect(rect, QColor("#D1D5DB")); // Light gray for track
        painter.setPen(QPen(QColor("#000000"), 1)); // Black border
        painter.drawRect(rect);
        break;

    case 2: // Normal wall
    case 4: // Wall with a bomb (disguised as a normal wall)
        if (tileImages.contains(2)) {
            // Draw the normal wall image for both tileType 2 and 4
            painter.drawImage(rect, tileImages[2]);
        }
        else {
            // Fallback if the image isn't available
            painter.fillRect(rect, QColor("#5B21B6")); // Purple for wall
            painter.setPen(QPen(QColor("#000000"), 1));
            painter.drawRect(rect);
        }
        break;

    case 3: // Player
        if (tileImages.contains(3)) {
            painter.drawImage(rect, tileImages[3]);
        }
        else {
            // Fallback for player
            painter.fillRect(rect, QColor("#10B981")); // Green for player
            painter.setPen(QPen(QColor("#000000"), 1));
            painter.drawRect(rect);
        }
        break;

    default: // Unknown tile type
        painter.fillRect(rect, QColor("#F3F4F6")); // Light gray for unknown
        painter.setPen(QPen(QColor("#000000"), 1));
        painter.drawRect(rect);
        break;
    }
}


