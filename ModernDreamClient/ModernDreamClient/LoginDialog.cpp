﻿#include "LoginDialog.h"
#include <QPixmap>
#include <QPalette>
#include <QBrush>
#include <QPainter>
#include <QMessageBox>
#include <QCoreApplication>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>

auto LoginDialog::createStorage()
{
    using namespace sqlite_orm;
    return make_storage("titan_vanguard_users.sqlite",
        make_table("users",
            make_column("username", &UserData::username, primary_key())));
}

LoginDialog::LoginDialog(QWidget* parent) : QDialog(parent)
{
    setWindowTitle("Titan Vanguard - Login");
    resize(600, 600);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setAlignment(Qt::AlignBottom);

    QWidget* loginPage = new QWidget();
    QVBoxLayout* loginLayout = new QVBoxLayout();
    loginLayout->setAlignment(Qt::AlignBottom | Qt::AlignHCenter);

    QFrame* frame = new QFrame(this);
    frame->setFixedSize(350, 120);
    frame->setStyleSheet(
        "QFrame {"
        "    background-color: rgba(30, 30, 30, 0.8);" // Gri-negru transparent
        "    border-radius: 15px;"
        "    border: 2px solid rgba(50, 50, 50, 0.9);"
        "}");

    QVBoxLayout* frameLayout = new QVBoxLayout(frame);
    frameLayout->setAlignment(Qt::AlignCenter);

    QLabel* usernameLabel = new QLabel("USERNAME:", this);
    usernameLabel->setStyleSheet("font-size: 14px; color: white; font-weight: bold; background: none; border: none; font-style: italic; letter-spacing: 2px;");
    usernameLabel->setAlignment(Qt::AlignCenter);

    usernameEdit = new QLineEdit(this);
    usernameEdit->setFixedWidth(300);
    usernameEdit->setFixedHeight(25);
    usernameEdit->setStyleSheet("background: rgba(255, 255, 255, 0.7); border-radius: 5px; padding: 5px;");
    usernameEdit->setAlignment(Qt::AlignCenter);

    QHBoxLayout* buttonLayout = new QHBoxLayout();
    loginButton = new QPushButton("LOGIN", this);
    registerButton = new QPushButton("REGISTER", this);

    loginButton->setFixedSize(100, 40);
    registerButton->setFixedSize(100, 40);

    loginButton->setStyleSheet(
        "QPushButton {"
        "    background-color: black;"
        "    color: purple;"
        "    border: 2px solid purple;"
        "    border-radius: 5px;"
        "    padding: 5px;"
        "}");
    registerButton->setStyleSheet(
        "QPushButton {"
        "    background-color: black;"
        "    color: purple;"
        "    border: 2px solid purple;"
        "    border-radius: 5px;"
        "    padding: 5px;"
        "}");

    buttonLayout->addWidget(loginButton);
    buttonLayout->addWidget(registerButton);
    buttonLayout->setAlignment(Qt::AlignCenter);

    frameLayout->addWidget(usernameLabel);
    frameLayout->addWidget(usernameEdit);
    frameLayout->addLayout(buttonLayout);

    loginLayout->addWidget(frame, 0, Qt::AlignBottom | Qt::AlignHCenter);

    loginPage->setLayout(loginLayout);

    QWidget* menuPage = new QWidget();
    QVBoxLayout* menuLayout = new QVBoxLayout();
    menuLayout->setAlignment(Qt::AlignCenter);

    QPushButton* singlePlayer = new QPushButton("Single Player", this);
    QPushButton* multiPlayer = new QPushButton("MultiPlayer", this);
    QPushButton* mapOptions = new QPushButton("Map Options", this);

    singlePlayer->setFixedSize(200, 50);
    multiPlayer->setFixedSize(200, 50);
    mapOptions->setFixedSize(200, 50);

    menuLayout->addStretch();
    menuLayout->addWidget(singlePlayer, 0, Qt::AlignCenter);
    menuLayout->addWidget(multiPlayer, 0, Qt::AlignCenter);
    menuLayout->addWidget(mapOptions, 0, Qt::AlignCenter);
    menuLayout->addStretch();

    menuPage->setLayout(menuLayout);

    stackedWidget = new QStackedWidget(this);
    stackedWidget->addWidget(loginPage);
    stackedWidget->addWidget(menuPage);

    mainLayout->addWidget(stackedWidget);
    setLayout(mainLayout);

    connect(loginButton, &QPushButton::clicked, this, &LoginDialog::OnLogin);
    connect(registerButton, &QPushButton::clicked, this, &LoginDialog::OnRegister);
    connect(singlePlayer, &QPushButton::clicked, this, &LoginDialog::onStartGame);
    connect(multiPlayer, &QPushButton::clicked, this, &LoginDialog::onOptions);
    connect(mapOptions, &QPushButton::clicked, this, &LoginDialog::onStartGame);
}



void LoginDialog::switchToMenu()
{
    stackedWidget->setCurrentIndex(1);
}

void LoginDialog::OnLogin()
{
    QString username = usernameEdit->text();

    if (username.isEmpty())
    {
        QMessageBox::warning(this, "Login Error", "Username cannot be empty");
        return;
    }

    auto storage = createStorage();
    storage.sync_schema();

    auto user = storage.get_pointer<UserData>(username.toStdString());
    if (user)
    {
        QMessageBox::information(this, "Login", "Successfully logged in!");
        switchToMenu();
    }
    else
    {
        QMessageBox::warning(this, "Login Error", "Invalid username");
    }
}

void LoginDialog::OnRegister()
{
    QString username = usernameEdit->text();

    if (username.isEmpty())
    {
        QMessageBox::warning(this, "Registration Error", "Username cannot be empty");
        return;
    }

    auto storage = createStorage();
    storage.sync_schema();

    if (storage.get_pointer<UserData>(username.toStdString()))
    {
        QMessageBox::warning(this, "Registration Error", "Username already exists");
        return;
    }

    UserData newUser{ username.toStdString() };
    storage.replace(newUser);

    QMessageBox::information(this, "Registration", "Successfully registered!");
}

void LoginDialog::onStartGame()
{
    QMessageBox::information(this, "Options", "Opening options menu...");
}

void LoginDialog::onOptions()
{
    QMessageBox::information(this, "Options", "Opening options menu...");
}

void LoginDialog::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);

    QPixmap backgroundImage;
    if (stackedWidget->currentIndex() == 0) {
        backgroundImage.load("../ModernDreamImages/Titans1.jpg");
    }
    else {
        backgroundImage.load("../ModernDreamImages/meniu1.jpeg");
    }

    if (!backgroundImage.isNull())
    {
        painter.drawPixmap(0, 0, width(), height(), backgroundImage.scaled(width(), height(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
    }
}
