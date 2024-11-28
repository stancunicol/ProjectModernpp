#pragma once
#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QMessageBox> // Pentru afișarea mesajelor de eroare

class PlayWindow : public QDialog
{
    Q_OBJECT

public:
    PlayWindow(QWidget* parent = nullptr); // Constructor
    ~PlayWindow(); // Destructor

protected:
    void OnGenerateClicked(); // Slot pentru gestionarea click-ului pe butonul de generare cod
    QString GenerateRandomNumber();// functie pentru generare random de cod
    void OnForwardClicked();

private:
    QLineEdit* generateCode = nullptr; // Câmp pentru numele de utilizator
    QPushButton* generateButton = nullptr; // Buton pentru logare
    QPushButton* forwardButton = nullptr; //Buton pentru mers mai departe
    QLabel* messageLabel = nullptr; // Etichetă pentru mesajele de feedback
};

