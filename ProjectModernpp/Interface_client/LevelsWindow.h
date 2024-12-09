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
    QPushButton* m_easyLevelButton= nullptr;
    QPushButton* m_mediumLevelButton = nullptr;
    QPushButton* m_hardLevelButton = nullptr;
    QLabel* m_messageLabel = nullptr;
    size_t level = 0;
};


