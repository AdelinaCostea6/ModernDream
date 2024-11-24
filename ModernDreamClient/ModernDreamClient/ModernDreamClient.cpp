#include "ModernDreamClient.h"

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

    connect(startGameButton, &QPushButton::clicked, this, &ModernDreamClient::onStartGame);
}
