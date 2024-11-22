#include "ModernDreamClient.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ModernDreamClient w;
    w.show();
    return a.exec();
}
