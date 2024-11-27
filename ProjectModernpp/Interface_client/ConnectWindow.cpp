#include "ConnectWindow.h"
#include <QVBoxLayout>
#include <QPushButton>
#include<qpalette>
ConnectWindow::ConnectWindow(QWidget* parent)
    : QWidget(parent) {

    // Setare titlu și dimensiune fereastră
    setWindowTitle("Join Room");
    setFixedSize(300, 200);


    QPalette palette;
    palette.setColor(QPalette::Window, Qt::black); // Fundal negru
    palette.setColor(QPalette::WindowText, Qt::white); // Text alb
    setPalette(palette);
    setAutoFillBackground(true); // Asigură-te că fundalul se umple cu culoarea


    QVBoxLayout* layout = new QVBoxLayout(this);
    QLabel* label = new QLabel("Insert code:", this);
    QFont font;
    font.setPointSize(14);  // Dimensiunea fontului
    font.setFamily("Segoe Script");  // Familia fontului
    font.setBold(true);  // Font îngroșat (opțional)

    label->setFont(font);
    layout->addWidget(label);
    // Câmp pentru codul camerei
    insertCode = new QLineEdit(this);
    layout->addWidget(insertCode);
    insertCode->setAlignment(Qt::AlignCenter);
    insertCode->setStyleSheet("color: lightBlue;background-color: darkBlue;font-size: 20px;border: 2px solid lightBlue");
    insertCode->setFixedSize(240, 50);
    layout->setAlignment(Qt::AlignCenter); // Centrarea layout-ului


    startButton = new QPushButton("START GAME", this);
    startButton->setStyleSheet("background-color:green;"
        "color:lightGreen;"
        "font-weight: bold;"
        "border-radius: 5px;"
        "font-size: 15px;"
        "padding: 10px;"
        "border: 2px solid darkGreen; ");
    layout->addWidget(startButton);
    connect(startButton, &QPushButton::clicked, this, &ConnectWindow::OnGenerateClicked);
}

void ConnectWindow::OnGenerateClicked()
{
    QString roomCode = insertCode->text();

    if (roomCode.isEmpty()) {
        QMessageBox::warning(this, "Error", "The code field is empty!");
    }
    else {
        QMessageBox::information(this, "Success", "You have joined the room: " + roomCode);
    }

}

ConnectWindow::~ConnectWindow() {
    // Destructor 
}
