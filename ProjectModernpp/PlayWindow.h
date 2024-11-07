#pragma once
#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QMessageBox> 
class PlayWindow : public QWidget {
    Q_OBJECT

public:
    PlayWindow(QWidget* parent = nullptr); // Constructor
private:
    QLineEdit* GenerateCode; 
    QPushButton* GenerateButton;
    QPushButton* FowardButton;
};

