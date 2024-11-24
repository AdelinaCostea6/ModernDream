#include "LoginDialog.h"
auto LoginDialog::createStorage()
{
    using namespace sqlite_orm;
    return make_storage("battle_city_users.sqlite",
        make_table("users",
            make_column("username", &UserData::username, primary_key()),
            make_column("password", &UserData::password)
        )
    );
}

LoginDialog::LoginDialog(QWidget* parent) : QDialog(parent)
{

    setWindowTitle("Battle City - Login");

    QVBoxLayout* layout = new QVBoxLayout(this);

    QLabel* titleLabel = new QLabel("Battle City Login", this);
    titleLabel->setStyleSheet("font-size: 20px; font-weight: bold;");
    layout->addWidget(titleLabel);

    QLabel* usernameLabel = new QLabel("Username:", this);
    usernameEdit = new QLineEdit(this);
    layout->addWidget(usernameLabel);
    layout->addWidget(usernameEdit);

    QLabel* passwordLabel = new QLabel("Password:", this);
    passwordEdit = new QLineEdit(this);
    passwordEdit->setEchoMode(QLineEdit::Password);
    layout->addWidget(passwordLabel);
    layout->addWidget(passwordEdit);

    QHBoxLayout* buttonLayout = new QHBoxLayout();
    loginButton = new QPushButton("Login", this);
    registerButton = new QPushButton("Register", this);

    buttonLayout->addWidget(loginButton);
    buttonLayout->addWidget(registerButton);
    layout->addLayout(buttonLayout);

    connect(loginButton, &QPushButton::clicked, this, &LoginDialog::onLogin);
    connect(registerButton, &QPushButton::clicked, this, &LoginDialog::onRegister);
}
