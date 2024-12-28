#pragma once

#include <QMainWindow>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QSpinBox>
#include <QComboBox>
#include <QPushButton>
#include <qlistwidget.h>
#include <qprogressbar.h>
#include "HttpClient.h"
#include <qstackedwidget.h>
enum class GameMap {
    CAR,
    HELICOPTER,
    BOAT
};
class ModernDreamClient : public QMainWindow
{
    Q_OBJECT

public:
    ModernDreamClient(QWidget* parent = nullptr);

private:
    QTabWidget* tabWidget;
    QSpinBox* playerCountSpinBox;
    QComboBox* mapComboBox;
    QPushButton* startGameButton;
    QWidget* waitingRoomWidget;
    QLabel* waitingStatusLabel;
    QProgressBar* playerProgress;  
    QListWidget* playerList; 
    QString currentUsername;
    HttpClient* httpClient;
    QStackedWidget* mainStack;
    GameMap currentMap;        
    QString currentSessionId;
    QWidget* gameWidget;

    void setupWaitingRoom();
    void updateWaitingRoom(int current, int required);

private slots:
    void OnStartGame(GameMap mapType, const QString& username); 

    void onJoinGameSuccess(const QString& sessionId, int current, int required);
    void onPlayerJoined(const QString& username, int current, int required);
    void onPlayerLeft(const QString& username, int current, int required);
    //void onGameReady(const QString& sessionId, const QJsonArray& players);
};