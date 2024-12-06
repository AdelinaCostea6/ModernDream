#pragma once

#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
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

    auto createStorage();

    void switchToMenu();

private slots:
    void OnLogin();
    void OnRegister();

};


