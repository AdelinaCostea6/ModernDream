#include "ModernDreamClient.h"
#include <QApplication>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>
#include <QTabWidget>
#include <sqlite_orm/sqlite_orm.h>

ModernDreamClient::ModernDreamClient(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
}

ModernDreamClient::~ModernDreamClient()
{
}

class LoginDialog : public QDiaglog
{
    Q_OBJECT

private:
    QLineEdit *usernameEdit;
    QLineEdit *passwordEdit;
    QPushButton *loginButton;
    QPushButton *registerButton;

    auto createStorage()
    {
        using namespace sqlite_orm;
        return make_storage("battle_city_users.sqlite",
                            make_table("users",
                                       make_column("username", &UserData::username,
                                                   primary_key()),
                                       make_column("password",
                                                   &UserData::password)));
    }

    struct UserData
    {
        std::string username;
        std::string password;
    };
}
