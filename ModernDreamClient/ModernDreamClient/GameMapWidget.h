#pragma once
#include <QWidget>
#include <QPixmap>
#include <QVector>

class GameMapWidget : public QWidget {
    Q_OBJECT
public:
    GameMapWidget(QWidget* parent = nullptr);
    void initializeMap(const QVector<QVector<int>>& mapData);
    void updatePlayerPosition(int x, int y);
  



protected:
    void paintEvent(QPaintEvent* event) override;

private:
    static const int CELL_SIZE = 40;
    QVector<QVector<int>> map;
    QPixmap wallTexture, bombTexture;
    QVector<QPixmap> playerTextures;
    void loadTextures();
};
