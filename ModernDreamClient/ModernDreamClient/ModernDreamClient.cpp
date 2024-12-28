﻿#include "ModernDreamClient.h"
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

    connect(startGameButton, &QPushButton::clicked, this, &ModernDreamClient::OnStartGame);
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
