/*#include "ModernDreamClient.h"
#include <QMessageBox.h>
#include "LoginDialog.h"
ModernDreamClient::ModernDreamClient(QWidget* parent)
{
    setWindowTitle("Battle City-Modern Dream");
    resize(800, 600);

    tabWidget = new QTabWidget(this);

    QWidget* gameSetupTab = new QWidget();
    QVBoxLayout* setupLayout = new QVBoxLayout(gameSetupTab);

    QLabel* playerCountLabel = new QLabel("Number of Players: ", this);
    playerCountSpinBox = new QSpinBox(this);
    playerCountSpinBox->setRange(1, 4);

    QLabel* mapLabel = new QLabel("Select Map:", this);
    mapComboBox = new QComboBox(this);
    mapComboBox->addItems({ "Small Map", "Medium Map", "Large Map" });

    startGameButton = new QPushButton("Start Game", this);

    setupLayout->addWidget(playerCountLabel);
    setupLayout->addWidget(playerCountSpinBox);
    setupLayout->addWidget(mapLabel);
    setupLayout->addWidget(mapComboBox);
    setupLayout->addWidget(startGameButton);

    tabWidget->addTab(gameSetupTab, "Game Setup");
    setCentralWidget(tabWidget);

    //connect(startGameButton, &QPushButton::clicked, this, &ModernDreamClient::OnStartGame);
    connect(startGameButton, &QPushButton::clicked, [this]() {
        QString selectedMap = mapComboBox->currentText();
        GameMap mapType;

        if (selectedMap == "Small Map") mapType = GameMap::CAR;
        else if (selectedMap == "Medium Map") mapType = GameMap::HELICOPTER;
        else if (selectedMap == "Large Map") mapType = GameMap::BOAT;

        QString username = "YourUsername";

        OnStartGame(mapType, username);
        });

}

//void ModernDreamClient::OnStartGame()
//{
//    int playerCount = playerCountSpinBox->value();
//    QString selectedMap = mapComboBox->currentText();
//
//
//    QMessageBox::information(this, "Game Start",
//        QString("Starting game with %1 players on map: %2")
//        .arg(playerCount)
//        .arg(selectedMap));
//
//}

void ModernDreamClient::setupWaitingRoom() {
    waitingRoomWidget = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(waitingRoomWidget);

    waitingStatusLabel = new QLabel("Waiting for players...");
    waitingStatusLabel->setAlignment(Qt::AlignCenter);
    waitingStatusLabel->setStyleSheet("font-size: 18px; color: #BF00FF;");

    playerProgress = new QProgressBar();
    playerProgress->setStyleSheet(
        "QProgressBar {"
        "   border: 2px solid #BF00FF;"
        "   border-radius: 5px;"
        "   text-align: center;"
        "}"
        "QProgressBar::chunk {"
        "   background-color: #BF00FF;"
        "}"
    );

    playerList = new QListWidget();
    playerList->setStyleSheet(
        "QListWidget {"
        "   border: 2px solid #BF00FF;"
        "   border-radius: 5px;"
        "   background: rgba(30, 30, 30, 0.8);"
        "}"
        "QListWidget::item {"
        "   color: white;"
        "   padding: 5px;"
        "}"
    );

    QPushButton* cancelButton = new QPushButton("Cancel");
    cancelButton->setStyleSheet(
        "QPushButton {"
        "   background-color: black;"
        "   color: #BF00FF;"
        "   border: 2px solid #BF00FF;"
        "   border-radius: 5px;"
        "   padding: 5px;"
        "   font-size: 12px;"
        "   font-weight: bold;"
        "}"
    );

    layout->addWidget(waitingStatusLabel);
    layout->addWidget(playerProgress);
    layout->addWidget(playerList);
    layout->addWidget(cancelButton);

    mainStack->addWidget(waitingRoomWidget);

    connect(cancelButton, &QPushButton::clicked, [this]() {
        httpClient->leaveGame(currentSessionId);
        //emit backToMapSelection();
        mainStack->setCurrentWidget(tabWidget);
        });
}

void ModernDreamClient::OnStartGame(GameMap mapType, const QString& username) { 
    currentUsername = username;
    currentMap = mapType;

    QString mapTypeStr;
    switch (mapType) {
    case GameMap::CAR: mapTypeStr = "car"; break;
    case GameMap::HELICOPTER: mapTypeStr = "helicopter"; break;
    case GameMap::BOAT: mapTypeStr = "boat"; break;
    }

    httpClient->joinGame(username, mapTypeStr, playerCountSpinBox->value());
    mainStack->setCurrentWidget(waitingRoomWidget);
}

void ModernDreamClient::onJoinGameSuccess(const QString& sessionId, int current, int required) {
    currentSessionId = sessionId;
    updateWaitingRoom(current, required);
    playerList->addItem(currentUsername + " (You)");
}

void ModernDreamClient::onPlayerJoined(const QString& username, int current, int required) {
    updateWaitingRoom(current, required);
    playerList->addItem(username);
}

void ModernDreamClient::onPlayerLeft(const QString& username, int current, int required) {
    updateWaitingRoom(current, required);
    for (int i = 0; i < playerList->count(); i++) {
        if (playerList->item(i)->text().startsWith(username)) {
            delete playerList->takeItem(i);
            break;
        }
    }
}

void ModernDreamClient::updateWaitingRoom(int current, int required) {
    waitingStatusLabel->setText(QString("Waiting for players... (%1/%2)").arg(current).arg(required));
    playerProgress->setMaximum(required);
    playerProgress->setValue(current);
}

//void ModernDreamClient::onGameReady(const QString& sessionId, const QJsonArray& players) {
//    // Initialize and start the actual game with all connected players
//    mainStack->setCurrentWidget(gameWidget);
//
//    // Create the game state
//    QVector<QString> playerNames;
//    for (const QJsonValue& player : players) {
//        playerNames.append(player.toString());
//    }
//
//    // Set up the game widget
//    gameWidget->initializeGame(currentMap, playerNames);
//    gameWidget->startGame();
//}
*/

#include "ModernDreamClient.h"
#include <QVBoxLayout>
#include <QMessageBox>
#include <QJsonArray>

ModernDreamClient::ModernDreamClient(QWidget* parent)
    : QMainWindow(parent), mainStack(new QStackedWidget(this)), httpClient(new HttpClient(this)) {
    setWindowTitle("Battle City - Modern Dream");
    resize(800, 600);

    tabWidget = new QTabWidget(this);

    QWidget* gameSetupTab = new QWidget();
    QVBoxLayout* setupLayout = new QVBoxLayout(gameSetupTab);

    QLabel* playerCountLabel = new QLabel("Number of Players: ", this);
    playerCountSpinBox = new QSpinBox(this);
    playerCountSpinBox->setRange(1, 4);

    QLabel* mapLabel = new QLabel("Select Map:", this);
    mapComboBox = new QComboBox(this);
    mapComboBox->addItems({ "Small Map", "Medium Map", "Large Map" });

    startGameButton = new QPushButton("Start Game", this);

    setupLayout->addWidget(playerCountLabel);
    setupLayout->addWidget(playerCountSpinBox);
    setupLayout->addWidget(mapLabel);
    setupLayout->addWidget(mapComboBox);
    setupLayout->addWidget(startGameButton);

    tabWidget->addTab(gameSetupTab, "Game Setup");

    mainStack->addWidget(tabWidget);
    setCentralWidget(mainStack);

    connect(startGameButton, &QPushButton::clicked, this, [this]() {
        OnStartGame(currentMap, currentUsername);
        });

    setupWaitingRoom();

    connect(httpClient, &HttpClient::gameReady, this, &ModernDreamClient::onGameReady);
    connect(httpClient, &HttpClient::playerJoined, this, &ModernDreamClient::onPlayerJoined);
    connect(httpClient, &HttpClient::playerLeft, this, &ModernDreamClient::onPlayerLeft);
    connect(httpClient, &HttpClient::joinGameSuccess, this, &ModernDreamClient::onJoinGameSuccess);
}

//void ModernDreamClient::setupWaitingRoom() {
//    waitingRoomWidget = new QWidget();
//    QVBoxLayout* layout = new QVBoxLayout(waitingRoomWidget);
//
//    waitingStatusLabel = new QLabel("Waiting for players...");
//    waitingStatusLabel->setAlignment(Qt::AlignCenter);
//
//    playerProgress = new QProgressBar();
//    playerList = new QListWidget();
//
//    QPushButton* leaveButton = new QPushButton("Leave Game");
//    connect(leaveButton, &QPushButton::clicked, this, &ModernDreamClient::onLeaveGame);
//
//    layout->addWidget(waitingStatusLabel);
//    layout->addWidget(playerProgress);
//    layout->addWidget(playerList);
//    layout->addWidget(leaveButton);
//
//    mainStack->addWidget(waitingRoomWidget);
//}
void ModernDreamClient::setupWaitingRoom() {
    waitingRoomWidget = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(waitingRoomWidget);
    layout->setAlignment(Qt::AlignCenter);

    // Create a frame for the waiting room content
    QFrame* waitingFrame = new QFrame();
    waitingFrame->setStyleSheet(
        "QFrame {"
        "    background-color: rgba(30, 30, 30, 0.8);"
        "    border: 2px solid #BF00FF;"
        "    border-radius: 15px;"
        "    padding: 20px;"
        "}");
    QVBoxLayout* frameLayout = new QVBoxLayout(waitingFrame);

    // Waiting status label
    waitingStatusLabel = new QLabel("Waiting for players...");
    waitingStatusLabel->setAlignment(Qt::AlignCenter);
    waitingStatusLabel->setStyleSheet(
        "color: #BF00FF;"
        "font-size: 24px;"
        "font-weight: bold;"
        "margin-bottom: 20px;"
    );

    // Progress bar
    playerProgress = new QProgressBar();
    playerProgress->setStyleSheet(
        "QProgressBar {"
        "    border: 2px solid #BF00FF;"
        "    border-radius: 5px;"
        "    text-align: center;"
        "    height: 25px;"
        "    margin: 10px 0px;"
        "}"
        "QProgressBar::chunk {"
        "    background-color: #BF00FF;"
        "}"
    );

    // Player list
    playerList = new QListWidget();
    playerList->setStyleSheet(
        "QListWidget {"
        "    border: 2px solid #BF00FF;"
        "    border-radius: 5px;"
        "    background: rgba(0, 0, 0, 0.7);"
        "    min-height: 200px;"
        "}"
        "QListWidget::item {"
        "    color: white;"
        "    padding: 10px;"
        "    margin: 5px;"
        "    background: rgba(191, 0, 255, 0.2);"
        "    border-radius: 5px;"
        "}"
    );

    // Leave button
    QPushButton* leaveButton = new QPushButton("Leave Game");
    leaveButton->setStyleSheet(
        "QPushButton {"
        "    background-color: black;"
        "    color: #BF00FF;"
        "    border: 2px solid #BF00FF;"
        "    border-radius: 5px;"
        "    padding: 10px 20px;"
        "    font-size: 16px;"
        "    font-weight: bold;"
        "    margin-top: 20px;"
        "}"
        "QPushButton:hover {"
        "    background-color: rgba(191, 0, 255, 0.2);"
        "}"
    );

    frameLayout->addWidget(waitingStatusLabel);
    frameLayout->addWidget(playerProgress);
    frameLayout->addWidget(playerList);
    frameLayout->addWidget(leaveButton);

    layout->addWidget(waitingFrame);

    connect(leaveButton, &QPushButton::clicked, this, &ModernDreamClient::onLeaveGame);

    mainStack->addWidget(waitingRoomWidget);
}

void ModernDreamClient::OnStartGame(GameMap mapType, const QString& username) {
    currentUsername = username;
    currentMap = mapType;

    QString mapTypeStr;
    switch (mapType) {
    case GameMap::CAR: mapTypeStr = "car"; break;
    case GameMap::HELICOPTER: mapTypeStr = "helicopter"; break;
    case GameMap::BOAT: mapTypeStr = "boat"; break;
    }

    httpClient->joinGame(username, mapTypeStr, playerCountSpinBox->value());
    mainStack->setCurrentWidget(waitingRoomWidget);
    qDebug() << "Adding waitingRoomWidget to mainStack";
    mainStack->addWidget(waitingRoomWidget);
    qDebug() << "Switching to waitingRoomWidget";
    mainStack->setCurrentWidget(waitingRoomWidget); 

}

void ModernDreamClient::onJoinGameSuccess(const QString& sessionId, int current, int required) {
    currentSessionId = sessionId;
    updateWaitingRoom(current, required);
    playerList->addItem(currentUsername + " (You)");
    mainStack->setCurrentWidget(waitingRoomWidget);
}

void ModernDreamClient::onPlayerJoined(const QString& username, int current, int required) {
    updateWaitingRoom(current, required);
    playerList->addItem(username);
}

void ModernDreamClient::onPlayerLeft(const QString& username, int current, int required) {
    updateWaitingRoom(current, required);
    for (int i = 0; i < playerList->count(); i++) {
        if (playerList->item(i)->text().startsWith(username)) {
            delete playerList->takeItem(i);
            break;
        }
    }
}

void ModernDreamClient::updateWaitingRoom(int current, int required) {
    waitingStatusLabel->setText(QString("Waiting for players... (%1/%2)").arg(current).arg(required));
    playerProgress->setMaximum(required);
    playerProgress->setValue(current);
}

void ModernDreamClient::onGameReady(const QString& sessionId, const QJsonArray& players) {
    mainStack->setCurrentWidget(gameWidget);
    QVector<QString> playerNames;
    for (const QJsonValue& player : players) {
        playerNames.append(player.toString());
    }

}
//void ModernDreamClient::onGameReady(const QString& sessionId, const QJsonArray& players) 
//{
//    // This will be called when all players have joined
//    QMessageBox::information(this, "Game Ready", "All players have joined! Starting game...");
//    // Here you would transition to your game screen
//}

//void ModernDreamClient::onLeaveGame() {
//    if (!currentSessionId.isEmpty()) {
//        httpClient->leaveGame(currentSessionId);
//        mainStack->setCurrentWidget(tabWidget);
//        playerList->clear();
//        waitingStatusLabel->setText("Waiting for players...");
//    }
//}
void ModernDreamClient::onLeaveGame() {
    if (!currentSessionId.isEmpty()) {
        httpClient->leaveGame(currentSessionId);
        mainStack->setCurrentWidget(tabWidget);
        playerList->clear();
        waitingStatusLabel->setText("Waiting for players...");
        playerProgress->setValue(0);
    }
}


