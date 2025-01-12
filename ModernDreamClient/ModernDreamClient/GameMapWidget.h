#pragma once
#include <QWidget>
#include <QPixmap>
#include <QVector>
#include <QMutex>
#include "HttpClient.h"

class GameMapWidget : public QWidget {
    Q_OBJECT
public:
    GameMapWidget(QWidget* parent = nullptr);
    void initializeMap(const QVector<QVector<int>>& mapData);
    void updatePlayerPosition(int x, int y);
    
  

public slots:
    void updateBullets(const QVector<BulletInfo>& bullets);
   


protected:
    void paintEvent(QPaintEvent* event) override;
    

private:
    static const int CELL_SIZE = 40;
    QVector<QVector<int>> map;
    QPixmap wallTexture, bombTexture,bulletTexture; 
    QVector<QPixmap> playerTextures;
    
    QVector<BulletInfo> bullets;
    QMutex bulletsMutex; 
    //std::mutex bulletsMutex;
    void loadTextures();
};
