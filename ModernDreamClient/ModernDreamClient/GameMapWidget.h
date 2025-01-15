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
    void setSessionId(const QString& sessionId);
   // void syncBulletsFromServer();
    ~GameMapWidget() {
        qDebug() << "GameMapWidget destructor called!";
    }
    
  

public slots:
    //void updateBullets(const QVector<BulletInfo>& bullets);
    void updateBullets(const QVector<QPair<int, int>>& newPositions/*, const QVector<char>& newDirections*/);
   


protected:
    void paintEvent(QPaintEvent* event) override;
    

private:
    bool syncInProgress = false;
    static const int CELL_SIZE = 40;
    QVector<QVector<int>> map;
    QPixmap wallTexture, bombTexture,bulletTexture; 
    QVector<QPixmap> playerTextures;
    
    //QVector<BulletInfo> bullets;
     QMutex bulletsMutex; 
    //std::mutex bulletsMutex;
    void loadTextures();

    QVector<QPair<int, int>> bulletPositions; // Stores positions of bullets
    //QVector<char> bulletDirections;  // Stores directions of bullets

    bool isPainting = false;
    int test = 1;
    QString currentSessionId;
    HttpClient *httpClient;

    QVector<QPair<int, int>> bulletBuffer;  // Buffer temporar
    QMutex bufferMutex;
    
};
