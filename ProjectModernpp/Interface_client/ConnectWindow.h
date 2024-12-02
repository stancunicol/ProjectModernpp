#pragma once

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QMessageBox> 

class ConnectWindow : public QDialog
{
    Q_OBJECT

public:
    ConnectWindow(QWidget* parent = nullptr); 
    ~ConnectWindow(); 

protected:
    void OnGenerateClicked(); 

private:
    QLineEdit* insertCode = nullptr; 
    QPushButton* startButton = nullptr; 
    QLabel* messageLabel = nullptr; 
};

