#pragma once
#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QMessageBox>
#include<qdebug>


class LevelsWindow : public QDialog
{
    Q_OBJECT

public:
    LevelsWindow(QWidget* parent = nullptr);
    ~LevelsWindow();
protected:
    void OnEasyLevelClicked();
    void OnMediumLevelClicked();
    void OnHardLevelClicked();

private:
    QPushButton* easyLevelButton= nullptr;
    QPushButton* mediumLevelButton = nullptr;
    QPushButton* hardLevelButton = nullptr;
    QLabel* messageLabel = nullptr;

};


