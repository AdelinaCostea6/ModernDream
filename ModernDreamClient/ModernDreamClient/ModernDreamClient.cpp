
#include "ModernDreamClient.h"
#include <QVBoxLayout>
#include <QMessageBox>
#include <QJsonArray>

ModernDreamClient::ModernDreamClient(QWidget* parent)
    : QMainWindow(parent), mainStack(new QStackedWidget(this)), httpClient(new HttpClient(this)) {
    resize(800, 600);

    
    setupWaitingRoom();
    qDebug() << "setupWaitingRoom initialized waitingRoomWidget:" << waitingRoomWidget;

    
    gameWidget = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(gameWidget);
    QLabel* gameLabel = new QLabel("Game is starting...", gameWidget);
    layout->addWidget(gameLabel);
    mainStack->addWidget(gameWidget);
    qDebug() << "gameWidget created and added to mainStack:" << gameWidget;

    
    tabWidget = new QTabWidget(this);
    QWidget* gameSetupTab = new QWidget();
    QVBoxLayout* setupLayout = new QVBoxLayout(gameSetupTab);

    QLabel* playerCountLabel = new QLabel("Number of Players: ", this);
    playerCountSpinBox = new QSpinBox(this);
    playerCountSpinBox->setRange(1, 4);

    startGameButton = new QPushButton("Start Game", this);

    setupLayout->addWidget(playerCountLabel);
    setupLayout->addWidget(playerCountSpinBox);
    setupLayout->addWidget(startGameButton);

    tabWidget->addTab(gameSetupTab, "Game Setup");
    mainStack->addWidget(tabWidget);
    qDebug() << "tabWidget created and added to mainStack:" << tabWidget;

    
    mainStack->setCurrentWidget(tabWidget);
    setCentralWidget(mainStack);

    
    connect(startGameButton, &QPushButton::clicked, this, [this]() {
        OnStartGame(currentMap, currentUsername);
        });

    
    connect(httpClient, &HttpClient::joinGameSuccess, this, &ModernDreamClient::onJoinGameSuccess);
    connect(httpClient, &HttpClient::gameReady, this, &ModernDreamClient::onGameReady); 
    connect(httpClient, &HttpClient::playerJoined, this, &ModernDreamClient::onPlayerJoined);
    connect(httpClient, &HttpClient::playerLeft, this, &ModernDreamClient::onPlayerLeft);

    qDebug() << "ModernDreamClient initialized successfully.";
}


ModernDreamClient::~ModernDreamClient() {
    qDebug() << "ModernDreamClient destroyed.";
    if (gameWidget) {
        qDebug() << "gameWidget still exists in destructor:" << gameWidget;
    }
}

void ModernDreamClient::setupWaitingRoom() {
    waitingRoomWidget = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(waitingRoomWidget);
    layout->setAlignment(Qt::AlignCenter);

 
    QFrame* waitingFrame = new QFrame();
    waitingFrame->setStyleSheet(
        "QFrame {"
        "    background-color: rgba(30, 30, 30, 0.8);"
        "    border: 2px solid #BF00FF;"
        "    border-radius: 15px;"
        "    padding: 20px;"
        "}");
    QVBoxLayout* frameLayout = new QVBoxLayout(waitingFrame);


    waitingStatusLabel = new QLabel("Waiting for players...");
    waitingStatusLabel->setAlignment(Qt::AlignCenter);
    waitingStatusLabel->setStyleSheet(
        "color: #BF00FF;"
        "font-size: 24px;"
        "font-weight: bold;"
        "margin-bottom: 20px;"
    );


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
  

}

void ModernDreamClient::onJoinGameSuccess(const QString& sessionId, int current, int required) {
    qDebug() << "onJoinGameSuccess called with sessionId:" << sessionId
        << "current:" << current << "required:" << required;
    currentSessionId = sessionId;
    updateWaitingRoom(current, required);
    playerList->addItem(currentUsername + " (You)");
    if (!waitingRoomWidget) {
        qDebug() << "Error: waitingRoomWidget is nullptr!";
        return;
    }

    
    if (mainStack->indexOf(waitingRoomWidget) == -1) {
        qDebug() << "Adding waitingRoomWidget to mainStack.";
        mainStack->addWidget(waitingRoomWidget);
    }
    mainStack->setCurrentWidget(waitingRoomWidget);
}

void ModernDreamClient::onPlayerJoined(const QString& username, int current, int required) {
    qDebug() << "Player joined:" << username;
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
    static bool gameStarted = false; 
    if (gameStarted) {
        qDebug() << "Game already started, ignoring additional calls.";
        return;
    }
    gameStarted = true;

    qDebug() << "onGameReady called. Session ID:" << sessionId;

    if (!gameWidget) {
        qDebug() << "Error: gameWidget is nullptr!";
        return;
    }

  
    QLayout* oldLayout = gameWidget->layout();
    if (oldLayout) {
        QLayoutItem* item;
        while ((item = oldLayout->takeAt(0)) != nullptr) {
            delete item->widget();
            delete item;
        }
        delete oldLayout;
    }

    QVBoxLayout* layout = new QVBoxLayout(gameWidget);
    layout->setContentsMargins(0, 0, 0, 0);  

    gameWidget->setStyleSheet("background-color: #62009e;"); 

    QWidget* topContainer = new QWidget(gameWidget);
    QVBoxLayout* topLayout = new QVBoxLayout(topContainer);


    QLabel* sessionLabel = new QLabel(QString("Session ID: %1").arg(sessionId), topContainer);
    sessionLabel->setAlignment(Qt::AlignRight);
    sessionLabel->setStyleSheet("font-size: 14px; color: white;");
    topLayout->addWidget(sessionLabel);


    QLabel* playersLabel = new QLabel("Players in the game:", topContainer);
    playersLabel->setStyleSheet("color: white;");
    topLayout->addWidget(playersLabel);


    for (const QJsonValue& player : players) {
        QLabel* playersNameLabel = new QLabel(player.toString(), topContainer);
        playersNameLabel->setStyleSheet("color: white");
        topLayout->addWidget(playersNameLabel);
       
    }

    layout->addWidget(topContainer);

 
    QLabel* gameCanvas = new QLabel("Game canvas goes here...", gameWidget);
    gameCanvas->setStyleSheet("background-color: #eedbf9; border: 1px solid black; height: 400px; margin-top: 20px;");
    gameCanvas->setAlignment(Qt::AlignCenter);
    layout->addWidget(gameCanvas, 1); 

    gameWidget->setLayout(layout);


    if (mainStack->indexOf(gameWidget) == -1) {
        mainStack->addWidget(gameWidget);
    }
    mainStack->setCurrentWidget(gameWidget);

    qDebug() << "Switched to gameWidget successfully.";
}


void ModernDreamClient::onLeaveGame() {
    if (!currentSessionId.isEmpty()) {
        httpClient->leaveGame(currentSessionId);
        mainStack->setCurrentWidget(tabWidget);
        playerList->clear();
        waitingStatusLabel->setText("Waiting for players...");
        playerProgress->setValue(0);
    }
}


