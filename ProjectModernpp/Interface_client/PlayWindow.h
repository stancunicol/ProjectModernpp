#pragma once
#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QMessageBox> // Pentru afișarea mesajelor de eroare

class PlayWindow : public QWidget {
    Q_OBJECT

public:
    PlayWindow(QWidget* parent = nullptr); // Constructor
    ~PlayWindow(); // Destructor

protected:
    void onGenerateClicked(); // Slot pentru gestionarea click-ului pe butonul de generare cod
    QString GenerateRandomNumber();// functie pentru generare random de cod

private:
    QLineEdit* GenerateCode; // Câmp pentru numele de utilizator
    QPushButton* GenerateButton; // Buton pentru logare
    QPushButton* FowardButton; //Buton pentru mers mai departe
    QLabel* messageLabel; // Etichetă pentru mesajele de feedback
};

