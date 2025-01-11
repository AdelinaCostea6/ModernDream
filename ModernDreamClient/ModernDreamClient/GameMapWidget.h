#pragma once
#include <QWidget>
#include <QPixmap>
#include <QVector>
#include "HttpClient.h"

class GameMapWidget : public QWidget {
    Q_OBJECT
public:
    GameMapWidget(QWidget* parent = nullptr);
    void initializeMap(const QVector<QVector<int>>& mapData);
    void updatePlayerPosition(int x, int y);
    //void updateBullets(const QVector<BulletInfo>& newBullets);
  

public slots:
    void updateBullets(const QVector<BulletInfo>& bullets); 


protected:
    void paintEvent(QPaintEvent* event) override;
    

private:
    static const int CELL_SIZE = 40;
    QVector<QVector<int>> map;
    QPixmap wallTexture, bombTexture;
    QVector<QPixmap> playerTextures;
    QPixmap bulletTexture;
    QVector<BulletInfo> bullets;
    std::mutex bulletsMutex;
    QTimer updateTimer; 
    bool updatingBullets = false;

    void loadTextures();
};
