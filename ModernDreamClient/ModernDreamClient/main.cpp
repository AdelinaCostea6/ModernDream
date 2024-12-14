//#include "ModernDreamClient.h"
//#include "LoginDialog.h"
//#include <QtWidgets/QApplication>
//
//int main(int argc, char *argv[])
//{
//    QApplication app(argc, argv);
//    LoginDialog loginDialog;
//   
//    /*if (loginDialog.exec() == QDialog::Accepted)
//    {
//        ModernDreamClient game;
//        game.show();
//        return app.exec();
//    }*/
//    loginDialog.show();
//    return app.exec();
//}
#include "ModernDreamClient.h"
#include <QApplication>
#include "LoginDialog.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv); 

    LoginDialog loginDialog;
    loginDialog.show();

    return app.exec(); 

}