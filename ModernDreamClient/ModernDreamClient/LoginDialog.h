#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H
#pragma once

#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>
#include <QStackedWidget> // Include QStackedWidget
#include <sqlite_orm/sqlite_orm.h>

struct UserData
{
    std::string username;
    std::string password;
};

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    LoginDialog(QWidget* parent = nullptr);

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    QPixmap background;
    QLineEdit* usernameEdit;
    QLineEdit* passwordEdit;
    QPushButton* loginButton;
    QPushButton* registerButton;

    QPixmap currentBackground;
    QWidget* loginView;
    QWidget* menuView;
    QStackedWidget* stackedWidget; 

    auto createStorage();

    void switchToMenu();

private slots:
    void OnLogin();
    void OnRegister();

    void onStartGame();
    void onOptions();

};
#endif