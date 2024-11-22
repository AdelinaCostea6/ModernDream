#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_ModernDreamClient.h"

class ModernDreamClient : public QMainWindow
{
    Q_OBJECT

public:
    ModernDreamClient(QWidget *parent = nullptr);
    ~ModernDreamClient();

private:
    Ui::ModernDreamClientClass ui;
};
