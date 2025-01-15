//#pragma once
//#include <QWidget>
//#include <QPixmap>
//#include <QVector>
//#include <QMutex>
//#include "HttpClient.h"
//
//class GameMapWidget : public QWidget {
//    Q_OBJECT
//public:
//    GameMapWidget(QWidget* parent = nullptr);
//    void initializeMap(const QVector<QVector<int>>& mapData);
//    void updatePlayerPosition(int x, int y);
//    void setSessionId(const QString& sessionId);
//   // void syncBulletsFromServer();
//    ~GameMapWidget() {
//        qDebug() << "GameMapWidget destructor called!";
//    }
//    
//  
//
//public slots:
//    //void updateBullets(const QVector<BulletInfo>& bullets);
//    void updateBullets(const QVector<QPair<int, int>>& newPositions/*, const QVector<char>& newDirections*/);
//   
//
//
//protected:
//    void paintEvent(QPaintEvent* event) override;
//    
//
//private:
//    bool syncInProgress = false;
//    static const int CELL_SIZE = 40;
//    QVector<QVector<int>> map;
//    QPixmap wallTexture, bombTexture,bulletTexture; 
//    QVector<QPixmap> playerTextures;
//    
//    //QVector<BulletInfo> bullets;
//     QMutex bulletsMutex; 
//    //std::mutex bulletsMutex;
//    void loadTextures();
//
//    QVector<QPair<int, int>> bulletPositions; // Stores positions of bullets
//    //QVector<char> bulletDirections;  // Stores directions of bullets
//
//    bool isPainting = false;
//    int test = 1;
//    QString currentSessionId;
//    HttpClient *httpClient;
//
//    QVector<QPair<int, int>> bulletBuffer;  // Buffer temporar
//    QMutex bufferMutex;
//    
//};
#pragma once
#include <QMainWindow>
#include <QTimer>
#include <QVector>
#include <QPixmap>
#include <QPair>
#include <QKeyEvent>
#include "HttpClient.h"

class GameMapWidget : public QMainWindow {
    Q_OBJECT
public:
    explicit GameMapWidget(const QString& sessionId, const QString& username, QWidget* parent = nullptr);

protected:
    void paintEvent(QPaintEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override; 

private:
    QString sessionId;
    QString username;
    HttpClient* httpClient;
    QVector<QVector<int>> mapData; // Stores map information
    QVector<QPair<int, int>> bullets; // Stores bullet positions
    QPixmap wallTexture, bombTexture, bulletTexture;
    QVector<QPixmap> playerTextures;

    void fetchAndInitializeMap(); // Fetch map data from the server and initialize
    void setupTextures();
    void setupConnections();

    void onShootButtonPressed(const QString& direction); 

public slots:
    void updateBullets(const QVector<QPair<int, int>>& bulletPositions);
    void onShootBullet(const QString& direction);

    void updatePlayerPosition(int x, int y); 
};
