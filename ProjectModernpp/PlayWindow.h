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

};

