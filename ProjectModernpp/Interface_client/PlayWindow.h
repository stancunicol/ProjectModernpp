#pragma once
#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QMessageBox> 

class PlayWindow : public QDialog
{
    Q_OBJECT

public:
    PlayWindow(QWidget* parent = nullptr); 
    ~PlayWindow(); 

protected:
    void OnGenerateClicked(); 
    QString GenerateRandomNumber();
    void OnForwardClicked();

private:
    QLineEdit* generateCode = nullptr; 
    QPushButton* generateButton = nullptr; 
    QPushButton* forwardButton = nullptr; 
    QLabel* messageLabel = nullptr; 
};

