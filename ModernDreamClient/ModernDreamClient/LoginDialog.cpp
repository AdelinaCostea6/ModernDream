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
/*
LoginDialog::LoginDialog(QWidget* parent) : QDialog(parent)
{
    
    setWindowTitle("Titan Vanguard - Login");

    resize(600, 600);

    background = QPixmap("../ModernDreamImages/Titans1.jpg");
    if (background.isNull()) {
        QMessageBox::warning(this, "Error", "Failed to load background image");
    }

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setAlignment(Qt::AlignCenter);

    QFrame* formFrame = new QFrame(this);
    formFrame->setFixedSize(350, 250);
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
    
    QLabel* titleLabel = new QLabel("Titan Vanguard Login", this);
    titleLabel->setStyleSheet("font-size: 24px; font-weight: bold; color: white;");
    titleLabel->setAlignment(Qt::AlignCenter);
    formLayout->addWidget(titleLabel);

    QVBoxLayout* inputLayout = new QVBoxLayout();
    QLabel* usernameLabel = new QLabel("USERNAME:", this);
    usernameLabel->setStyleSheet("font-size: 14px; color: white; font-weight:bold; background: none; border:none; font-style:italic; letter-spacing:2px;");
    usernameEdit = new QLineEdit(this);
    usernameEdit->setFixedWidth(300);
    inputLayout->addWidget(usernameLabel);
    inputLayout->addWidget(usernameEdit);
    inputLayout->setSpacing(10);
    inputLayout->setAlignment(Qt::AlignCenter);
    formLayout->addLayout(inputLayout);
    
    QLabel* passwordLabel = new QLabel("Password:", this);
    passwordEdit = new QLineEdit(this);
    passwordEdit->setEchoMode(QLineEdit::Password);
    inputLayout->addWidget(passwordLabel);
    inputLayout->addWidget(passwordEdit);
    
    
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
    setLayout(mainLayout);

    connect(loginButton, &QPushButton::clicked, this, &LoginDialog::OnLogin);
    connect(registerButton, &QPushButton::clicked, this, &LoginDialog::OnRegister);
}
*/
LoginDialog::LoginDialog(QWidget* parent) : QDialog(parent)
{
    setWindowTitle("Titan Vanguard - Login");

    resize(600, 600);

    // Load background image
    background = QPixmap("../ModernDreamImages/Titans1.jpg");
    if (background.isNull()) {
        QMessageBox::warning(this, "Error", "Failed to load background image");
    }

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setAlignment(Qt::AlignCenter);

    QFrame* formFrame = new QFrame(this);
    formFrame->setFixedSize(350, 300);
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

    // Add title
    QLabel* titleLabel = new QLabel("Titan Vanguard Login", this);
    titleLabel->setStyleSheet("font-size: 24px; font-weight: bold; color: white;");
    titleLabel->setAlignment(Qt::AlignCenter);
    formLayout->addWidget(titleLabel);
    formLayout->addStretch(); // Push input fields to the bottom

    // Input fields layout
    QVBoxLayout* inputLayout = new QVBoxLayout();
    QLabel* usernameLabel = new QLabel("USERNAME:", this);
    usernameLabel->setStyleSheet("font-size: 14px; color: white; font-weight: bold; background: none; border: none; font-style: italic; letter-spacing: 2px;");
    usernameEdit = new QLineEdit(this);
    usernameEdit->setFixedWidth(300);
    usernameEdit->setStyleSheet("background: rgba(255, 255, 255, 0.7); border-radius: 5px; padding: 5px;");

    QLabel* passwordLabel = new QLabel("PASSWORD:", this);
    passwordLabel->setStyleSheet("font-size: 14px; color: white; font-weight: bold; background: none; border: none; font-style: italic; letter-spacing: 2px;");
    passwordEdit = new QLineEdit(this);
    passwordEdit->setEchoMode(QLineEdit::Password);
    passwordEdit->setFixedWidth(300);
    passwordEdit->setStyleSheet("background: rgba(255, 255, 255, 0.7); border-radius: 5px; padding: 5px;");

    inputLayout->addWidget(usernameLabel);
    inputLayout->addWidget(usernameEdit);
    inputLayout->addWidget(passwordLabel);
    inputLayout->addWidget(passwordEdit);
    inputLayout->setSpacing(15);
    inputLayout->setAlignment(Qt::AlignCenter);

    formLayout->addLayout(inputLayout);

    // Buttons layout
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
    setLayout(mainLayout);

    // Connect buttons to slots
    connect(loginButton, &QPushButton::clicked, this, &LoginDialog::OnLogin);
    connect(registerButton, &QPushButton::clicked, this, &LoginDialog::OnRegister);
}


void LoginDialog::paintEvent(QPaintEvent* event) 
{
    QPainter painter(this);

    if (!background.isNull()) {
        painter.drawPixmap(0, 0, width(), height(), background.scaled(width(), height(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
    }
}
/*
void LoginDialog::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);

    if (!background.isNull()) {
        painter.drawPixmap(0, 0, width(), height(), background.scaled(width(), height(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
    }

    // Draw the form frame with a more visually appealing style
    
    QRect formRect(width() / 2 - 175, height() / 2 - 150, 350, 300);
    painter.setPen(QPen(QColor(255, 255, 255, 100), 2));
    painter.setBrush(QBrush(QColor(50, 50, 50, 200)));
    painter.drawRoundedRect(formRect, 20, 20);

    // Draw the "Titan Vanguard Login" text with a larger font
    
    QFont titleFont("Arial", 28, QFont::Bold);
    painter.setFont(titleFont);
    painter.setPen(QPen(Qt::white));
    painter.drawText(formRect.adjusted(0, 20, 0, 0), Qt::AlignHCenter | Qt::AlignTop, "Titan Vanguard Login");

    // Draw the username and password input fields
    QFont inputFont("Arial", 16);
    painter.setFont(inputFont);
    painter.drawText(formRect.adjusted(30, 80, -30, 0), Qt::AlignLeft | Qt::AlignVCenter , "USERNAME:");
    painter.drawText(formRect.adjusted(30, 130, -30, 0), Qt::AlignLeft | Qt::AlignVCenter , "PASSWORD:");

    // Draw the login and register buttons with a more appealing style
    QRect buttonRect(formRect.left() + 40, formRect.bottom() - 80, 130, 40);
    painter.setBrush(QBrush(QColor(100, 100, 255, 200)));
    painter.drawRoundedRect(buttonRect, 10, 10);
    painter.drawText(buttonRect, Qt::AlignCenter, "Enter");

    buttonRect.moveLeft(formRect.right() - 170);
    painter.setBrush(QBrush(QColor(255, 100, 100, 200)));
    painter.drawRoundedRect(buttonRect, 10, 10);
    painter.drawText(buttonRect, Qt::AlignCenter, "Join");
}
*/
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