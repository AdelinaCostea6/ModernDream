#pragma once

#include <QMainWindow>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QSpinBox>
#include <QComboBox>
#include <QPushButton>

class ModernDreamClient : public QMainWindow
{
    Q_OBJECT

private:
    QTabWidget* tabWidget;
    QSpinBox* playerCountSpinBox;
    QComboBox* mapComboBox;
    QPushButton* startGameButton;

public:
    ModernDreamClient(QWidget* parent = nullptr);

private slots:
    void OnStartGame();
};