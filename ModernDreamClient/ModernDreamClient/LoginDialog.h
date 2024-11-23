#pragma once

#include <QDialog>
#include <QVBoxLayout>
#include <QLabel>
#include <QlineEdit>
#include <QPushbutton>
#include <QMessageBox>
#include <sqlite_orm/sqlite_orm.h>

struct UserData
{
    std::string username;
    std::string password;
};

class LoginDialog : public QDialog
{
    Q_OBJECT

private:
    QLineEdit *usernameEdit;
    QLineEdit *passwordEdit;
    QPushButton *loginButton;
    QPushButton *registerButton;

    auto createStorage();

public:
    LoginDialog(QWidget *parent = nullptr);

private slots:
    void onLogin();
    void onRegister();
};