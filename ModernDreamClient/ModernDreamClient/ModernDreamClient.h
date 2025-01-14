
#pragma once

#include <QMainWindow>
#include <QStackedWidget>
#include <QTabWidget>
#include <QSpinBox>
#include <QComboBox>
#include <QPushButton>
#include <QProgressBar>
#include <QListWidget>
#include <QLabel>
#include <QString>
#include "HttpClient.h"
#include "GameMapWidget.h"
enum class GameMap {
    CAR,
    HELICOPTER,
    BOAT
};

class ModernDreamClient : public QMainWindow {
    Q_OBJECT

public:
    explicit ModernDreamClient(QWidget* parent = nullptr);
    ~ModernDreamClient();

protected:
    void paintEvent(QPaintEvent* event) override
    {
        gameWidget->update();
    }


private:
    QTabWidget* tabWidget;          
    QSpinBox* playerCountSpinBox;   
    QComboBox* mapComboBox;        
    QPushButton* startGameButton;  

      
    QWidget* waitingRoomWidget;     
    QWidget* gameWidget;            

    QLabel* waitingStatusLabel;  
    QProgressBar* playerProgress;   
    QListWidget* playerList;        

    QString currentUsername;
    QString currentSessionId;       
    GameMap currentMap;             

    HttpClient* httpClient;        

    QStackedWidget* mainStack;
    GameMapWidget* mapWidget;
    QString currentDirection;
    void setupWaitingRoom();       
   // bool isSyncing = false;

protected:
    void keyPressEvent(QKeyEvent* event) override;
    

public slots:
    void OnStartGame(GameMap mapType, const QString& username);
    void onJoinGameSuccess(const QString& sessionId, int current, int required);
    void onPlayerJoined(const QString& username, int current, int required);
    void onPlayerLeft(const QString& username, int current, int required);
    void onGameReady(const QString& sessionId, const QJsonArray& players);
    //void onGameReady(const QString& sessionId, const QJsonArray& players, const QJsonObject& mapData); 
    void onLeaveGame(); 
    void updateWaitingRoom(int current, int required);
    void updatePlayerPosition(int x, int y);

    void onShootButtonPressed(const QString& direction);
};