#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_ModernDreamClient.h"
#include <QlineEdit>
#include <QPushbutton>

class ModernDreamClient : public QMainWindow
{
    Q_OBJECT

public:
    ModernDreamClient(QWidget *parent = nullptr);
    ~ModernDreamClient();

    LoginDialog(QWidget *parent = nullptr);

private:
    Ui::ModernDreamClientClass ui;
    QLineEdit *usernameEdit;
    QLineEdit *passwordEdit;
    QPushButton *loginButton;
    QPushButton *registerButton;
    auto CreateStorage();
    struct UserData
    {
        std::string username;
        std::string password;
    };

private slots:
    void OnLogin();
    void OnRegister();
};