
#include "ModernDreamClient.h"
#include <QApplication>
#include "LoginDialog.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv); 

    LoginDialog loginDialog;
    loginDialog.show();

    return app.exec(); 

   
        //QApplication app(argc, argv);

        //// Setăm numărul de jucători
        //int requiredPlayers = 1;
        //QVector<LoginDialog*> loginDialogs; // Vector pentru a păstra instanțele deschise

        //// Creează mai multe ferestre LoginDialog
        //for (int i = 0; i < requiredPlayers; ++i) {
        //    LoginDialog* loginDialog = new LoginDialog();
        //    loginDialog->setWindowTitle(QString("Login/Register - Player %1").arg(i + 1));
        //    loginDialog->show();
        //    loginDialogs.push_back(loginDialog); // Salvăm instanța pentru a nu fi ștearsă automat
        //}

        //return app.exec();
    

}