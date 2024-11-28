#pragma once

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QMessageBox> // Pentru afișarea mesajelor de eroare

class ConnectWindow : public QDialog
{
    Q_OBJECT

public:
    ConnectWindow(QWidget* parent = nullptr); // Constructor
    ~ConnectWindow(); // Destructor

protected:
    void OnGenerateClicked(); // Slot pentru gestionarea click-ului pe butonul de generare cod

private:
    QLineEdit* insertCode = nullptr; // Câmp pentru numele de utilizator
    QPushButton* startButton = nullptr; //Buton pentru mers mai departe
    QLabel* messageLabel = nullptr; // Etichetă pentru mesajele de feedback
};

