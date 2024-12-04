#include "LoginDialog.h"
#include <QPixmap>
#include <QPalette>
#include <QBrush>
#include <QPainter>

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

    background = QPixmap("../ModernDreamImages/Titans1.jpg");
    if (background.isNull()) {
        QMessageBox::warning(this, "Error", "Failed to load background image");
    }

    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    QSpacerItem* topSpacer = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);
    mainLayout->addSpacerItem(topSpacer);

    QHBoxLayout* centerLayout = new QHBoxLayout();
    centerLayout->setAlignment(Qt::AlignCenter);

    QFrame* formFrame = new QFrame(this);
    formFrame->setFixedSize(350, 120);
    formFrame->setStyleSheet(
        "QFrame {"
        "    background: qlineargradient("
        "        spread:pad, x1:0, y1:0, x2:1, y2:1, "
        "        stop:0 rgba(0, 0, 0, 157), stop:1 rgba(50, 50, 50, 157) "
        "    );"
        "    border-radius: 10px;"
        "    border: 1px solid rgba(255, 255, 255, 50);"
        "}"
    );

    QVBoxLayout* formLayout = new QVBoxLayout(formFrame);

    QVBoxLayout* inputLayout = new QVBoxLayout();
    QLabel* usernameLabel = new QLabel("USERNAME:", this);
    usernameLabel->setStyleSheet("font-size: 14px; color: white; font-weight: bold; background: none; border: none; font-style: italic; letter-spacing: 2px;");
    usernameEdit = new QLineEdit(this);
    usernameEdit->setFixedWidth(300);
    usernameEdit->setFixedHeight(25);
    usernameEdit->setStyleSheet("background: rgba(255, 255, 255, 0.7); border-radius: 5px; padding: 5px;");

    inputLayout->addWidget(usernameLabel);
    inputLayout->addWidget(usernameEdit);
    inputLayout->setSpacing(10);
    inputLayout->setAlignment(Qt::AlignCenter);

    formLayout->addLayout(inputLayout);

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
        "    font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "    background-color: rgba(0, 0, 0, 0.8);"
        "    color: white;"
        "    border-color: white;"
        "    font-weight: bold;"
        "}"
        "QPushButton:pressed {"
        "    background-color: purple;"
        "    color: black;"
        "    border-color: black;"
        "    font-weight: bold;"
        "}"
    );

    registerButton->setStyleSheet( 
        "QPushButton {"
        "    background-color: black;"
        "    color: purple;"
        "    border: 2px solid purple;"
        "    border-radius: 5px;"
        "    padding: 5px;"
        "    font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "    background-color: rgba(0, 0, 0, 0.8);"
        "    color: white;"
        "    border-color: white;"
        "    font-weight: bold;"
        "}"
        "QPushButton:pressed {"
        "    background-color: purple;"
        "    color: black;"
        "    border-color: black;"
        "    font-weight: bold;"
        "}"
    ); 

    buttonLayout->addWidget(loginButton);
    buttonLayout->addWidget(registerButton);
    buttonLayout->setSpacing(20);
    buttonLayout->setAlignment(Qt::AlignCenter);

    formLayout->addLayout(buttonLayout);

    centerLayout->addWidget(formFrame);

    mainLayout->addLayout(centerLayout);

    setLayout(mainLayout);

    connect(loginButton, &QPushButton::clicked, this, &LoginDialog::OnLogin);
    connect(registerButton, &QPushButton::clicked, this, &LoginDialog::OnRegister);
}


void LoginDialog::paintEvent(QPaintEvent* event) 
{
    QPainter painter(this);

    if (!background.isNull()) {
        painter.drawPixmap(0, -90, width(), height()+90, background.scaled(width(), height(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
    }
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