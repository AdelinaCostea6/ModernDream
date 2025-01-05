#pragma once

#include <QWidget>
#include <QPainter>
#include <QMap>
#include <QImage>
#include <QTimer>

class GameMapWidget : public QWidget {
    Q_OBJECT

public:
    explicit GameMapWidget(QWidget* parent = nullptr);

    void setMapData(const QVector<QVector<int>>& data);

    void setTileSize(int size);
    void setTileImages(const QMap<int, QImage>& images);

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    int tileSize;
    QVector<QVector<int>> mapData;

    QMap<int, QImage> tileImages; // Mapping of tile types to images
    QColor trackColor;
    void drawTile(QPainter& painter, int tileType, const QRect& rect);
    
};
