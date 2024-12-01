#include "LoginDialog.h"
#include <QPixmap>
#include <QPalette>
#include <QBrush>

auto LoginDialog::createStorage()
{
    using namespace sqlite_orm;
    return make_storage("titan_vanguard_users.sqlite",
        make_table("users",
            make_column("username", &UserData::username, primary_key()),
            make_column("password", &UserData::password)
        )
    );
}

LoginDialog::LoginDialog(QWidget* parent) : QDialog(parent)
{
    setWindowTitle("Titan Vanguard - Login");

    QPixmap background("C:/Users/Roxana/Desktop/mc/ModernDream/ModernDreamImages/Titans.jpeg");
    QPalette palette;
    palette.setBrush(QPalette::Window, QBrush(background));
    setPalette(palette);
    setAutoFillBackground(true);

    showFullScreen();

    QVBoxLayout* layout = new QVBoxLayout(this);

    QLabel* titleLabel = new QLabel("Titan Vanguard Login", this);
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

    connect(loginButton, &QPushButton::clicked, this, &LoginDialog::OnLogin);
    connect(registerButton, &QPushButton::clicked, this, &LoginDialog::OnRegister);
}

void LoginDialog::OnLogin()
{
    QString username = usernameEdit->text();
    QString password = passwordEdit->text();

    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Login Error", "Username and password cannot be empty");
        return;
    }

    auto storage = createStorage();
    storage.sync_schema();

    auto user = storage.get_pointer<UserData>(username.toStdString());
    if (user && user->password == password.toStdString()) {
        QMessageBox::information(this, "Login", "Successfully logged in!");
        accept();
    }
    else {
        QMessageBox::warning(this, "Login Error", "Invalid username or password");
    }
}

void LoginDialog::OnRegister()
{
    QString username = usernameEdit->text();
    QString password = passwordEdit->text();

    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Registration Error", "Username and password cannot be empty");
        return;
    }

    auto storage = createStorage();
    storage.sync_schema();

    if (storage.get_pointer<UserData>(username.toStdString())) {
        QMessageBox::warning(this, "Registration Error", "Username already exists");
        return;
    }

    UserData newUser{ username.toStdString(), password.toStdString() };
    storage.replace(newUser);

    QMessageBox::information(this, "Registration", "Successfully registered!");
}