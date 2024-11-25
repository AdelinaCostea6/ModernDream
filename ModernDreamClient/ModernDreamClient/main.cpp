#include "ModernDreamClient.h"
#include "LoginDialog.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    LoginDialog loginDialog;
    
    if (loginDialog.exec() == QDialog::Accepted)
    {
        ModernDreamClient game;
        game.show();
        return app.exec();
    }
}
