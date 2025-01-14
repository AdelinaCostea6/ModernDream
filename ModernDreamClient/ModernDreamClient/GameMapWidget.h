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

    //~GameMapWidget() {  
    //    QMutexLocker lock(&bulletsMutex); 
    //    bullets.clear();  // Ensure bullets are cleared safely
    //}

    
  

public slots:
    //void updateBullets(const QVector<BulletInfo>& bullets);
    void updateBullets(const QVector<QPair<int, int>>& newPositions, const QVector<char>& newDirections);
   


protected:
    void paintEvent(QPaintEvent* event) override;
    

private:
    static const int CELL_SIZE = 40;
    QVector<QVector<int>> map;
    QPixmap wallTexture, bombTexture,bulletTexture; 
    QVector<QPixmap> playerTextures;
    
    //QVector<BulletInfo> bullets;
    mutable QMutex bulletsMutex; 
    //std::mutex bulletsMutex;
    void loadTextures();

    QVector<QPair<int, int>> bulletPositions; // Stores positions of bullets
    QVector<char> bulletDirections;  // Stores directions of bullets

    bool isPainting = false;
    int test = 1;
    
};
