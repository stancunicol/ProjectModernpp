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
    QLineEdit* m_generateCode = nullptr; 
    QPushButton* m_generateButton = nullptr; 
    QPushButton* m_forwardButton = nullptr; 
    QLabel* m_messageLabel = nullptr; 
};

