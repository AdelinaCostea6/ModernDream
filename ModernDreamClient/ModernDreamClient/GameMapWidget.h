
#pragma once
#include <QMainWindow>
#include <QTimer>
#include <QVector>
#include <QPixmap>
#include <QPair>
#include <QKeyEvent>
#include "HttpClient.h"
#include "BulletInfo.h"

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
    QVector<QVector<int>> mapData; 
    QPixmap wallTexture, bombTexture, bulletTexture;
    QVector<QPixmap> playerTextures;
    QSharedPointer<QVector<BulletInfo>> bullets;
    QString currentDirection;

    void fetchAndInitializeMap(); 
    void setupTextures();
    void setupConnections();
    void displayMap();

   
    QMutex bulletsMutex;
    void syncBullets(const QString& sessionId);
    bool isUpdating = false;
    void shootBullet(const QString& direction);
    void updateMapCells(const QVector<QPair<int, int>>& updatedCells);
    QMutex mapMutex;
    void updateWalls();
    void syncPlayers();
    void movePlayer(const QString& direction);


    QVector<QPair<int, int>> updatedCells;
    QMap<QString, QPoint> playerPositions;

    QMap<QString, QPair<int, int>> playersPositions;


    //void notifyServerBombTriggered(int bombX, int bombY);

    

    QTransform transform;

public slots:

    void updatePlayerPosition(int x, int y); 


};
