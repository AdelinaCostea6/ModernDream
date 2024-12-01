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
   
    resize(600, 600);
   
    QPixmap background("../ModernDreamImages/Titans1.jpg");
    if (!background.isNull()) {
        background = background.scaled(size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
    }
    else {
        QMessageBox::warning(this, "Error", "Failed to load background image");
    }
    
    QPalette palette;
    palette.setBrush(QPalette::Window, QBrush(background));
    setPalette(palette);
    setAutoFillBackground(true);
  
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setAlignment(Qt::AlignCenter);
   
    QFrame* formFrame = new QFrame(this);
    formFrame->setFixedSize(350, 250); 
    formFrame->setStyleSheet(
        "QFrame {"
        "    background: qlineargradient("
        "        spread:pad, x1:0, y1:0, x2:1, y2:1, "
        "        stop:0 rgba(0, 0, 0, 157), stop:1 rgba(50, 50, 50, 157)"
        "    );"
        "    border-radius: 10px;"
        "    border: 1px solid rgba(255, 255, 255, 50);"
        "}"
    );
    QVBoxLayout* formLayout = new QVBoxLayout(formFrame);

    QLabel* titleLabel = new QLabel("Titan Vanguard Login", this);
    titleLabel->setStyleSheet("font-size: 24px; font-weight: bold; color: white;");
    titleLabel->setAlignment(Qt::AlignCenter);
    formLayout->addWidget(titleLabel);
   

    QVBoxLayout* inputLayout = new QVBoxLayout();
    QLabel* usernameLabel = new QLabel("USERNAME:", this);
    usernameLabel->setStyleSheet("font-size: 14px; color: white; font-weight:bold; background: none; border:none; font-style:italic; letter-spacing:2px;");
    usernameEdit = new QLineEdit(this);
    usernameEdit->setFixedWidth(300);
   // QLabel* passwordLabel = new QLabel("PASSWORD:", this);
    //passwordLabel->setStyleSheet("font-size: 14px; color: white;");
    //passwordEdit = new QLineEdit(this);
    //passwordEdit->setEchoMode(QLineEdit::Password);
    //passwordEdit->setFixedWidth(300);
    inputLayout->addWidget(usernameLabel);
    inputLayout->addWidget(usernameEdit);
   // inputLayout->addWidget(passwordLabel);
    //inputLayout->addWidget(passwordEdit);
    inputLayout->setSpacing(10);
    inputLayout->setAlignment(Qt::AlignCenter);
    formLayout->addLayout(inputLayout);
 
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    loginButton = new QPushButton("Login", this);
    registerButton = new QPushButton("Register", this);
    loginButton->setFixedSize(100, 30);
    registerButton->setFixedSize(100, 30);
    buttonLayout->addWidget(loginButton);
    buttonLayout->addWidget(registerButton);
    buttonLayout->setSpacing(20);
    buttonLayout->setAlignment(Qt::AlignCenter);
    formLayout->addLayout(buttonLayout);
    
    mainLayout->addWidget(formFrame);
   
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