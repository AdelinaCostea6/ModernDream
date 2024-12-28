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
#include <QStackedWidget>
#include <sqlite_orm/sqlite_orm.h>
#include "HttpClient.h"
#include "ModernDreamClient.h"

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    LoginDialog(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    HttpClient *httpClient;
    QLineEdit *usernameEdit;
    QPushButton *loginButton;
    QPushButton *registerButton;

    QPixmap background;
    QPixmap currentBackground;

    QWidget *loginView;
    QWidget *menuView;
    QStackedWidget *stackedWidget;
    ModernDreamClient* modernDreamClient;

    auto createStorage();

    void switchToMenu();

private slots:
    void OnLogin();
    void OnRegister();
    //void onStartGame();
    void onOptions();
    void onHelicopterSelected();
    void onBoatSelected();
    void onCarSelected();
    void onLoginSuccess(const QString &username, int score);
    void onRegisterSuccess();
    void onLoginFailure(const QString &error);
    void onRegisterFailure(const QString &error);
    void showMessageDialog(const QString &message, const QString &color);
    void onJoinGameSuccess(const QString& sessionId, int currentPlayers, int requiredPlayers);
    void onJoinGameFailure(const QString& error);

signals:
    void switchToWaitingRoom(const QString& sessionId, int currentPlayers, int requiredPlayers);


};
#endif